from typing import List

from . import Constituent

def known() -> List[str]:
    ...


def parse(name: str) -> Constituent:
    ...


def name(ident: Constituent) -> str:
    ...
