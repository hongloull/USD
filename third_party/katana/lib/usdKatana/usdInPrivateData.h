//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#ifndef PXRUSDKATANA_USDIN_PRIVATEDATA_H
#define PXRUSDKATANA_USDIN_PRIVATEDATA_H

#include "pxr/pxr.h"
#include "usdKatana/usdInArgs.h"

#include "pxr/usd/usd/prim.h"
#include <FnGeolib/op/FnGeolibOp.h>
#include <memory>

PXR_NAMESPACE_OPEN_SCOPE


/// \brief Private data for each non-root invocation of \c PxrUsdIn. 
///
/// \sa PxrUsdKatanaUsdInArgs
class PxrUsdKatanaUsdInPrivateData : public Foundry::Katana::GeolibPrivateData
{

public:
    /// \brief Material specialization hierarchy for Usd shading.
    struct MaterialHierarchy {
        std::map<SdfPath, SdfPath> baseMaterialPath;
        // Maintain order of derivedMaterials, for presentation.
        std::map<SdfPath, std::vector<SdfPath>> derivedMaterialPaths;
    };

    PxrUsdKatanaUsdInPrivateData(
            const UsdPrim& prim,
            PxrUsdKatanaUsdInArgsRefPtr usdInArgs,
            const PxrUsdKatanaUsdInPrivateData* parentData = NULL);

    virtual ~PxrUsdKatanaUsdInPrivateData()
    {
    }

    const UsdPrim& GetUsdPrim() const {
        return _prim;
    }

    const PxrUsdKatanaUsdInArgsRefPtr GetUsdInArgs() const {
        return _usdInArgs;
    }

    const SdfPath& GetInstancePath() const {
        return _instancePath;
    }

    const SdfPath& GetMasterPath() const {
        return _masterPath;
    }

    const double GetCurrentTime() const {
        return _currentTime;
    }

    const double GetShutterOpen() const {
        return _shutterOpen;
    }

    const double GetShutterClose() const {
        return _shutterClose;
    }

    /// \brief Return true if motion blur is backward.
    ///
    /// PxrUsdIn supports both forward and backward motion blur. Motion
    /// blur is considered backward if multiple samples are requested
    /// and the first specified sample is later than the last sample.
    const bool IsMotionBackward() const;

    const std::vector<double> GetMotionSampleTimes(
        const UsdAttribute& attr = UsdAttribute()) const;

private:

    UsdPrim _prim;

    PxrUsdKatanaUsdInArgsRefPtr _usdInArgs;

    SdfPath _instancePath;
    SdfPath _masterPath;

    double _currentTime;
    double _shutterOpen;
    double _shutterClose;

    std::vector<double> _motionSampleTimesOverride;
    std::vector<double> _motionSampleTimesFallback;

};


PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXRUSDKATANA_USDIN_PRIVATEDATA_H
