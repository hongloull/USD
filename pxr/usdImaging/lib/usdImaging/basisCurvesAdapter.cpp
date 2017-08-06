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
#include "pxr/usdImaging/usdImaging/basisCurvesAdapter.h"

#include "pxr/usdImaging/usdImaging/delegate.h"
#include "pxr/usdImaging/usdImaging/tokens.h"

#include "pxr/imaging/hd/basisCurves.h"
#include "pxr/imaging/hd/perfLog.h"
#include "pxr/imaging/hd/renderIndex.h"

#include "pxr/usd/usdGeom/basisCurves.h"
#include "pxr/usd/usdGeom/xformCache.h"

#include "pxr/base/tf/type.h"

PXR_NAMESPACE_OPEN_SCOPE


TF_REGISTRY_FUNCTION(TfType)
{
    typedef UsdImagingBasisCurvesAdapter Adapter;
    TfType t = TfType::Define<Adapter, TfType::Bases<Adapter::BaseAdapter> >();
    t.SetFactory< UsdImagingPrimAdapterFactory<Adapter> >();
}

UsdImagingBasisCurvesAdapter::~UsdImagingBasisCurvesAdapter() 
{
}

bool
UsdImagingBasisCurvesAdapter::IsSupported(HdRenderIndex* renderIndex)
{
    return renderIndex->IsRprimTypeSupported(HdPrimTypeTokens->basisCurves);
}

SdfPath
UsdImagingBasisCurvesAdapter::Populate(UsdPrim const& prim, 
                            UsdImagingIndexProxy* index,
                            UsdImagingInstancerContext const* instancerContext)
{
    index->InsertBasisCurves(prim.GetPath(),
                             GetShaderBinding(prim),
                             instancerContext);
    HD_PERF_COUNTER_INCR(UsdImagingTokens->usdPopulatedPrimCount);

    return prim.GetPath();
}

void 
UsdImagingBasisCurvesAdapter::TrackVariabilityPrep(UsdPrim const& prim,
                                             SdfPath const& cachePath,
                                             HdDirtyBits requestedBits,
                                             UsdImagingInstancerContext const* 
                                                 instancerContext)
{
    // Let the base class track what it needs.
    BaseAdapter::TrackVariabilityPrep(
        prim, cachePath, requestedBits, instancerContext);
}

void 
UsdImagingBasisCurvesAdapter::TrackVariability(UsdPrim const& prim,
                                               SdfPath const& cachePath,
                                               HdDirtyBits requestedBits,
                                               HdDirtyBits* dirtyBits,
                                               UsdImagingInstancerContext const* 
                                                   instancerContext)
{
    BaseAdapter::TrackVariability(
        prim, cachePath, requestedBits, dirtyBits, instancerContext);

    if (requestedBits & HdChangeTracker::DirtyPoints) {
        // Discover time-varying points.
        _IsVarying(prim, 
                   UsdGeomTokens->points, 
                   HdChangeTracker::DirtyPoints,
                   UsdImagingTokens->usdVaryingPrimVar,
                   dirtyBits,
                   /*isInherited*/false);
    }
    if (requestedBits & HdChangeTracker::DirtyTopology) {
        // Discover time-varying topology.
        //
        // Note that basis, wrap and type are all uniform attributes, so they can't
        // vary over time.
        _IsVarying(prim, UsdGeomTokens->curveVertexCounts,
                           HdChangeTracker::DirtyTopology,
                           UsdImagingTokens->usdVaryingTopology,
                           dirtyBits, 
                           /*isInherited*/false);
    }
    if (requestedBits & HdChangeTracker::DirtyWidths) {
        _IsVarying(prim, UsdGeomTokens->widths,
                           HdChangeTracker::DirtyWidths,
                           UsdImagingTokens->usdVaryingWidths,
                           dirtyBits, 
                           /*isInherited*/false);
    }
    if (requestedBits & HdChangeTracker::DirtyNormals) {
        _IsVarying(prim, UsdGeomTokens->normals,
                           HdChangeTracker::DirtyNormals,
                           UsdImagingTokens->usdVaryingNormals,
                           dirtyBits, 
                           /*isInherited*/false);
    }
}

void 
UsdImagingBasisCurvesAdapter::UpdateForTimePrep(UsdPrim const& prim,
                                   SdfPath const& cachePath, 
                                   UsdTimeCode time,
                                   HdDirtyBits requestedBits,
                                   UsdImagingInstancerContext const* 
                                       instancerContext)
{
    BaseAdapter::UpdateForTimePrep(
        prim, cachePath, time, requestedBits, instancerContext);
    UsdImagingValueCache* valueCache = _GetValueCache();

    if (requestedBits & HdChangeTracker::DirtyPoints)
        valueCache->GetPoints(cachePath);

    if (requestedBits & HdChangeTracker::DirtyTopology)
        valueCache->GetTopology(cachePath);

    if (requestedBits & HdChangeTracker::DirtyWidths)
        valueCache->GetWidths(cachePath);

    if (requestedBits & HdChangeTracker::DirtyNormals)
        valueCache->GetNormals(cachePath);
}

void 
UsdImagingBasisCurvesAdapter::UpdateForTime(UsdPrim const& prim,
                               SdfPath const& cachePath, 
                               UsdTimeCode time,
                               HdDirtyBits requestedBits,
                               HdDirtyBits* resultBits,
                               UsdImagingInstancerContext const* 
                                   instancerContext)
{
    BaseAdapter::UpdateForTime(
        prim, cachePath, time, requestedBits, resultBits, instancerContext);
    UsdImagingValueCache* valueCache = _GetValueCache();

    PrimvarInfoVector& primvars = valueCache->GetPrimvars(cachePath);
    if (requestedBits & HdChangeTracker::DirtyTopology) {
        VtValue& topology = valueCache->GetTopology(cachePath);
        _GetBasisCurvesTopology(prim, &topology, time);
    }

    if (requestedBits & HdChangeTracker::DirtyPoints) {
        VtValue& points = valueCache->GetPoints(cachePath);
        _GetPoints(prim, &points, time);
        UsdImagingValueCache::PrimvarInfo primvar;
        primvar.name = HdTokens->points;
        primvar.interpolation = UsdGeomTokens->vertex;
        _MergePrimvar(primvar, &primvars);
    }

    if (requestedBits & HdChangeTracker::DirtyWidths) {
        UsdImagingValueCache::PrimvarInfo primvar;
        UsdGeomBasisCurves curves(prim);
        VtFloatArray widths;
        primvar.name = UsdGeomTokens->widths;
        if (curves.GetWidthsAttr().Get(&widths, time)) {
            primvar.interpolation = UsdGeomTokens->vertex;
        } else {
            widths = VtFloatArray(1);
            widths[0] = 1.0f;
            primvar.interpolation = UsdGeomTokens->constant;
        }
        _MergePrimvar(primvar, &primvars);
        valueCache->GetWidths(cachePath) = VtValue(widths);
    }
    if (requestedBits & HdChangeTracker::DirtyNormals) {
        UsdImagingValueCache::PrimvarInfo primvar;
        UsdGeomBasisCurves curves(prim);
        VtVec3fArray normals;
        primvar.name = UsdGeomTokens->normals;
        if (curves.GetNormalsAttr().Get(&normals, time)) {
            primvar.interpolation = UsdGeomTokens->vertex;
            _MergePrimvar(primvar, &primvars);
            valueCache->GetNormals(cachePath) = VtValue(normals);
        } else {
            *resultBits &= ~HdChangeTracker::DirtyNormals;
        }
    }
}


// -------------------------------------------------------------------------- //

void
UsdImagingBasisCurvesAdapter::_GetBasisCurvesTopology(UsdPrim const& prim, 
                                         VtValue* topo,
                                         UsdTimeCode time)
{
    HD_TRACE_FUNCTION();
    HF_MALLOC_TAG_FUNCTION();

    // These are uniform attributes and can't vary over time.
    UsdTimeCode unvarying = UsdTimeCode::Default();
    TfToken topoCurveBasis, topoCurveType, topoCurveWrap;
    TfToken curveBasis = _Get<TfToken>(prim, UsdGeomTokens->basis, unvarying);
    TfToken curveType = _Get<TfToken>(prim, UsdGeomTokens->type, unvarying);
    TfToken curveWrap = _Get<TfToken>(prim, UsdGeomTokens->wrap, unvarying);

    if(curveBasis == UsdGeomTokens->bezier) {
        topoCurveBasis = HdTokens->bezier;
    }
    else if(curveBasis == UsdGeomTokens->bspline) {
        topoCurveBasis = HdTokens->bSpline;
    }
    else if(curveBasis == UsdGeomTokens->catmullRom) {
        topoCurveBasis = HdTokens->catmullRom;
    }
    else {
        TF_CODING_ERROR("Unknown curve basis '%s'", curveBasis.GetText());
    }

    if(curveType == UsdGeomTokens->linear) {
        topoCurveType = HdTokens->linear;
    }
    else if(curveType == UsdGeomTokens->cubic) {
        topoCurveType = HdTokens->cubic;
    }
    else {
        TF_CODING_ERROR("Unknown curve type '%s'", curveType.GetText());
    }

    if(curveWrap == UsdGeomTokens->periodic) {
        topoCurveWrap = HdTokens->periodic;
    }
    else if(curveWrap == UsdGeomTokens->nonperiodic) {
        topoCurveWrap = HdTokens->nonperiodic;
    }
    else {
        TF_CODING_ERROR("Unknown curve wrap '%s'", curveWrap.GetText());
    }

    HdBasisCurvesTopology topology(
        topoCurveType, topoCurveBasis, topoCurveWrap,
        _Get<VtIntArray>(prim, UsdGeomTokens->curveVertexCounts, time),
        VtIntArray());
    *topo = VtValue(topology);
}

void
UsdImagingBasisCurvesAdapter::_GetPoints(UsdPrim const& prim, 
                                   VtValue* value, 
                                   UsdTimeCode time)
{
    HD_TRACE_FUNCTION();
    if (!prim.GetAttribute(UsdGeomTokens->points).Get(value, time)) {
        TF_WARN("Points could not be read from prim: <%s>",
                prim.GetPath().GetText());
        *value = VtVec3fArray();
    }
}

PXR_NAMESPACE_CLOSE_SCOPE

