# Include.
find_path(AATOOLKIT_INCLUDE_DIR
    NAMES
    AaBox
    AaBuffer
    AaCentroid
    AaCmdLine
    AaColor
    AaEigen3x3
    AaException
    AaGeometry
    AaGridIterator
    AaImage
    AaInterpolator
    AaMatrix
    AaPolar
    AaRatio
    AaSignal
    AaSmartPointer
    AaTable
    AaText
    AaTimer
    AaUmeyama
    AaVector
    AaXml
    bits/AaBox.hh
    bits/AaBuffer.hh
    bits/AaCentroid.hh
    bits/AaCmdLine.hh
    bits/AaColor.hh
    bits/AaEigen3x3.hh
    bits/AaException.hh
    bits/AaGeometry.hh
    bits/AaGridIterator.hh
    bits/AaImage.hh
    bits/AaInterpolator.hh
    bits/AaMatrix.hh
    bits/AaPolar.hh
    bits/AaPromote.hh
    bits/AaRatio.hh
    bits/AaSignal.hh
    bits/AaSmartPointer.hh
    bits/AaTable.hh
    bits/AaText.hh
    bits/AaTimer.hh
    bits/AaToolkit.h
    bits/AaUmeyama.hh
    bits/AaVector.hh
    bits/AaXml.hh
    PATHS
    ${AATOOLKIT_PATH}/include
)

if(AATOOLKIT_INCLUDE_DIR)
    set(AATOOLKIT_FOUND TRUE)
endif(AATOOLKIT_INCLUDE_DIR)

if(AATOOLKIT_FOUND)
    if(NOT AATOOLKIT_FIND_QUIETLY)
        message(STATUS "Found AaToolkit : ${AATOOLKIT_INCLUDE_DIR}")
    endif(NOT AATOOLKIT_FIND_QUIETLY)
else(AATOOLKIT_FOUND)
    if(AATOOLKIT_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find AaToolkit")
    endif(AATOOLKIT_FIND_REQUIRED)
endif(AATOOLKIT_FOUND)
