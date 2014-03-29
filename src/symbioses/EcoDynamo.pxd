from AquaticModel cimport AquaticModel as _AquaticModel
from AquaticModel cimport BaseAquaticModel

cdef extern from "EcoDynamoAquaticModel.h":
    cdef cppclass EcoDynamoAquaticModel(_AquaticModel):
        EcoDynamoAquaticModel()

cdef class AquaticModel(BaseAquaticModel):
    pass

