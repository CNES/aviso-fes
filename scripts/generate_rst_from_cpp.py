#!/usr/bin/env python3
"""
Script to automatically generate RST file for tidal constituents from C++
header file.

This script parses the wave.hpp file and extracts constituent documentation
to generate corresponding RST entries for the constituent.rst file.
"""

import argparse
from pathlib import Path
import re
import sys

#: Root directory of the project
ROOT = Path(__file__).resolve().parent.parent


class ConstituentParser:
    """Parser for extracting tidal constituent information from C++ header."""

    def __init__(self, cpp_header_path: str, constituent_header_path: str):
        self.cpp_header_path = Path(cpp_header_path)
        self.constituent_header_path = Path(constituent_header_path)
        self.constituent_to_class_map: dict[str, str] = {}
        self._load_constituent_mapping()

    def _load_constituent_mapping(self):
        """Load mapping from enum values to class names from constituent.hpp."""
        try:
            with open(self.constituent_header_path) as f:
                content = f.read()

            # Extract enum values and their math representations
            enum_pattern = r'k([A-Za-z0-9_]+),\s*//!<\s*@f\$([^@]+)@f\$'
            matches = re.findall(enum_pattern, content)

            for enum_name, math_repr in matches:
                # Map enum name to math representation
                self.constituent_to_class_map[enum_name] = math_repr.strip()

        except FileNotFoundError:
            print(f"Warning: Could not find {self.constituent_header_path}")

    def parse_constituents(self) -> list[dict]:
        """Parse all tidal constituents from the C++ header file."""
        try:
            with open(self.cpp_header_path) as f:
                content = f.read()
        except FileNotFoundError:
            print(f"Error: Could not find {self.cpp_header_path}")
            return []

        constituents = []

        # Split content by class definitions to process each one individually
        class_pattern = (r'/// @brief @f\$([^@]+)@f\$(?:\s*\([^)]*\))?\s*\n'
                         r'///\s*\n'
                         r'/// <table>.*?'
                         r'/// </table>\s*\n'
                         r'((?:/// @[^@]*?)*?)class\s+([A-Za-z0-9_]+)\s*:')

        matches = re.finditer(class_pattern, content, re.DOTALL)

        for match in matches:
            math_name = match.group(1).strip()
            notes_section = match.group(2) if match.group(2) else ''
            class_name = match.group(3)

            # Extract the full table section for this constituent
            full_match_text = content[match.start():match.end()]

            # Extract table information
            table_match = re.search(
                r'/// <table>.*?'
                r'/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>\s*\n'
                r'/// <tr><td>@f\$([^@]+)@f\$</td>\s*\n'
                r'/// <td>@f\$([^@]+)@f\$</td>\s*\n'
                r'/// <td>@f\$([^@]+)@f\$</td></tr>\s*\n'
                r'/// </table>', full_match_text, re.DOTALL)

            if table_match:
                v_value = table_match.group(1).strip()
                u_value = table_match.group(2).strip()
                factor_f = table_match.group(3).strip()

                # Extract note if present
                note = None
                if notes_section:
                    note_match = re.search(
                        r'/// @note\s+(.+?)(?=\n///[^@]|\n(?!///)|\nclass|$)',
                        notes_section, re.DOTALL)
                    if note_match:
                        note = note_match.group(1).strip()
                        # Clean up the note text
                        note = re.sub(r'\s+', ' ', note)
                        note = note.replace('%Table', 'Table').replace('%', '')
                        # Remove extra /// prefixes
                        note = re.sub(r'/// ?', '', note)

                # Find corresponding enum name
                enum_name = None
                for enum, math_repr in self.constituent_to_class_map.items():
                    normalized_class = self._normalize_class_name(enum)
                    if normalized_class == class_name:
                        enum_name = f"k{enum}"
                        break

                if not enum_name:
                    # Try to guess enum name from class name
                    if class_name.startswith('_'):
                        enum_name = f"k{class_name[1:]}"
                    else:
                        enum_name = f"k{class_name}"

                constituents.append({
                    'enum_name': enum_name,
                    'math_name': math_name,
                    'class_name': class_name,
                    'v_value': v_value,
                    'u_value': u_value,
                    'factor_f': factor_f,
                    'note': note
                })

        return constituents

    def _normalize_class_name(self, enum_name: str) -> str:
        """Convert enum name to expected class name."""
        # Handle special cases
        if enum_name.startswith('2'):
            return f"_{enum_name}"
        elif enum_name.startswith('3'):
            return f"_{enum_name}"
        elif enum_name == 'MSqm':
            return 'MSqm'
        elif enum_name == 'Sigma1':
            return 'Sigma1'
        elif enum_name == 'Rho1':
            return 'Rho1'
        elif enum_name == 'Chi1':
            return 'Chi1'
        elif enum_name == 'Theta1':
            return 'Theta1'
        elif enum_name == 'Pi1':
            return 'Pi1'
        elif enum_name == 'Phi1':
            return 'Phi1'
        elif enum_name == 'Psi1':
            return 'Psi1'
        elif ('MNS2' in enum_name or 'MS2' in enum_name or 'MN2' in enum_name
              or 'SM2' in enum_name or 'MK3' in enum_name or 'MK6' in enum_name
              or 'MS4' in enum_name or 'MS6' in enum_name or 'MS8' in enum_name
              or 'MN4' in enum_name or 'MN6' in enum_name
              or 'MSN4' in enum_name or 'MSN6' in enum_name
              or 'MNS4' in enum_name):
            if enum_name.startswith('2') or enum_name.startswith('3'):
                return f"_{enum_name}"

        return enum_name

    def generate_rst_content(self, constituents: list[dict]) -> str:
        """Generate RST content for all constituents."""
        rst_lines = []

        rst_lines.append('.. currentmodule:: pyfes.core')
        rst_lines.append('')
        rst_lines.append('.. class:: Constituent')
        rst_lines.append('')
        rst_lines.append(
            '    This enum class encapsulates the tidal constituents '
            'known by the library.')
        rst_lines.append('')
        rst_lines.append('    .. method:: __init__(self, value)')
        rst_lines.append('')
        rst_lines.append('        Constructor.')
        rst_lines.append('')
        rst_lines.append('        :param value: The value of the constituent.')
        rst_lines.append('')

        for constituent in constituents:
            rst_lines.extend(self._generate_constituent_rst(constituent))

        return '\n'.join(rst_lines)

    def _generate_constituent_rst(self, constituent: dict) -> list[str]:
        """Generate RST content for a single constituent."""
        lines = []

        # Attribute directive
        lines.append(f"    .. autoattribute:: {constituent['enum_name']}")
        lines.append('')

        # Math name
        lines.append(f"        :math:`{constituent['math_name']}`")
        lines.append('')

        # Table
        # Calculate column widths
        v_width = max(len('V'), len(f":math:`{constituent['v_value']}`"))
        u_width = max(len('u'), len(f":math:`{constituent['u_value']}`"))
        factor_width = max(len('Factor-f'),
                           len(f":math:`{constituent['factor_f']}`"))

        # Ensure minimum width and make all equal
        max_width = max(v_width, u_width, factor_width, 10)

        # Create table separator
        separator = '=' * max_width
        table_sep = (f"        {separator}  {separator}  "
                     f'{separator}')

        lines.append(table_sep)
        lines.append(f"        {'V':<{max_width}}  {'u':<{max_width}}  "
                     f"{'Factor-f':<{max_width}}")
        lines.append(table_sep)
        v_cell = f":math:`{constituent['v_value']}`"
        u_cell = f":math:`{constituent['u_value']}`"
        f_cell = f":math:`{constituent['factor_f']}`"
        lines.append(f"        {v_cell:<{max_width}}  {u_cell:<{max_width}}  "
                     f"{f_cell:<{max_width}}")
        lines.append(table_sep)
        lines.append('')

        # Note (if present)
        if constituent['note']:
            lines.append('        .. note::')
            lines.append('')
            lines.append(f"            {constituent['note']}")
            lines.append('')

        return lines


def main():
    """Main function to run the generator."""
    parser = argparse.ArgumentParser(
        description='Generate RST file for tidal constituents from "'
        '"C++ header file.',
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    wave_path = ROOT / 'include' / 'fes' / 'wave.hpp'
    constituent_path = ROOT / 'include' / 'fes' / 'constituent.hpp'
    output_rst = ROOT / 'docs' / 'source' / 'core' / 'constituent.rst'
    parser.add_argument('--verbose',
                        '-v',
                        action='store_true',
                        help='Enable verbose output')

    args = parser.parse_args()

    if not wave_path.exists():
        print(f"Error: Wave header file not found: {wave_path}")
        sys.exit(1)

    if not constituent_path.exists():
        print(f"Error: Constituent header file not found: {constituent_path}")
        sys.exit(1)

    parser_obj = ConstituentParser(str(wave_path), str(constituent_path))
    constituents = parser_obj.parse_constituents()

    if not constituents:
        print('No constituents found!')
        sys.exit(1)

    if args.verbose:
        print(f"Found {len(constituents)} constituents:")
        for c in constituents:
            note_info = ' (with note)' if c['note'] else ' (no note)'
            print(f"  - {c['enum_name']}: {c['math_name']}{note_info}")
    else:
        print(f"Found {len(constituents)} constituents")

    rst_content = parser_obj.generate_rst_content(constituents)

    # Write to output file
    output_rst.parent.mkdir(parents=True, exist_ok=True)

    with open(output_rst, 'w') as f:
        f.write(rst_content)

    print(f"Generated RST file: {output_rst}")

    if args.verbose:
        print(f"Output file size: {output_rst.stat().st_size} bytes")


if __name__ == '__main__':
    main()
