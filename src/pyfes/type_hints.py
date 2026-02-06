# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
""".. rubric:: Type aliases.

.. py:data:: VectorInt8
    :canonical: VectorInt8

    A vector of :py:class:`numpy.int8`.

.. py:data:: VectorUInt8
    :canonical: VectorUInt8

    A vector of :py:class:`numpy.uint8`.

.. py:data:: VectorUInt16
    :canonical: VectorUInt16

    A vector of :py:class:`numpy.uint16`.

.. py:data:: VectorInt64
    :canonical: VectorInt64

    A vector of :py:class:`numpy.int64`.

.. py:data:: VectorFloat64
    :canonical: VectorFloat64

    A vector of :py:class:`numpy.float64`.

.. py:data:: VectorComplex64
    :canonical: VectorComplex64

    A vector of :py:class:`numpy.complex64`.

.. py:data:: VectorComplex128
    :canonical: VectorComplex128

    A vector of :py:class:`numpy.complex128`.

.. py:data:: VectorDateTime64
    :canonical: VectorDateTime64

    A vector of :py:class:`numpy.datetime64`.

.. py:data:: MatrixInt32
    :canonical: MatrixInt32

    A matrix of :py:class:`numpy.int32`.

.. py:data:: MatrixFloat64
    :canonical: MatrixFloat64

    A matrix of :py:class:`numpy.float64`.

.. py:data:: NDArrayStructured
    :canonical: NDArrayStructured

    A numpy array whose data type is a composition of simpler data types
    organised as a sequence of named fields.
"""

from typing import TYPE_CHECKING, Annotated, Any, Literal, TypeVar
from types import GenericAlias


import numpy

if TYPE_CHECKING:
    from numpy.typing import NDArray

    DType = TypeVar('DType', bound=numpy.generic)
    Vector = Annotated[NDArray[DType], Literal['N']]
    Matrix = Annotated[NDArray[DType], Literal['N', 'M']]

    VectorInt8 = Vector[numpy.int8]
    VectorUInt8 = Vector[numpy.uint8]
    VectorUInt16 = Vector[numpy.uint16]
    VectorInt64 = Vector[numpy.int64]
    VectorFloat64 = Vector[numpy.float64]
    VectorComplex64 = Vector[numpy.complex64]
    VectorComplex128 = Vector[numpy.complex128]
    VectorDateTime64 = Vector[numpy.datetime64]
    MatrixInt32 = Matrix[numpy.int32]
    MatrixFloat64 = Matrix[numpy.float64]
    MatrixComplex128 = Matrix[numpy.complex128]
    NDArrayStructured = numpy.ndarray[Any, numpy.dtype[numpy.void]]
else:
    ScalarType_co = TypeVar(
        'ScalarType_co', bound=numpy.generic, covariant=True
    )
    DType = GenericAlias(numpy.dtype, (ScalarType_co,))

    Vector = GenericAlias(numpy.ndarray, (Any, DType))
    Matrix = GenericAlias(numpy.ndarray, (Any, DType))
    VectorInt8 = GenericAlias(numpy.ndarray, (Any, DType))
    VectorUInt8 = GenericAlias(numpy.ndarray, (Any, DType))
    VectorUInt16 = GenericAlias(numpy.ndarray, (Any, DType))
    VectorInt64 = GenericAlias(numpy.ndarray, (Any, DType))
    VectorFloat64 = GenericAlias(numpy.ndarray, (Any, DType))
    VectorComplex64 = GenericAlias(numpy.ndarray, (Any, DType))
    VectorComplex128 = GenericAlias(numpy.ndarray, (Any, DType))
    VectorDateTime64 = GenericAlias(numpy.ndarray, (Any, DType))
    MatrixInt32 = GenericAlias(numpy.ndarray, (Any, DType))
    MatrixFloat64 = GenericAlias(numpy.ndarray, (Any, DType))
    MatrixComplex128 = GenericAlias(numpy.ndarray, (Any, DType))
    NDArrayStructured = GenericAlias(
        numpy.ndarray, (Any, numpy.dtype[numpy.void])
    )
