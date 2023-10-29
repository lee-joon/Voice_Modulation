/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/

#ifndef Voice_ModulatorFXParams_H
#define Voice_ModulatorFXParams_H

#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <AK/Plugin/PluginServices/AkFXParameterChangeHandler.h>

// Add parameters IDs here, those IDs should map to the AudioEnginePropertyID
// attributes in the xml property definition.
static const AkPluginParamID PARAM_PLACEHOLDER_ID   = 0;
static const AkPluginParamID PARAM_Treshold_ID      = 1;
static const AkPluginParamID PARAM_Aattack_ID       = 2;
static const AkPluginParamID PARAM_Release_ID       = 3;
static const AkPluginParamID PARAM_Sat_Param_ID     = 4;
static const AkPluginParamID PARAM_Filter_Q_ID      = 5;
static const AkPluginParamID PARAM_Filter_Freq_ID   = 6;
static const AkPluginParamID PARAM_Modular_Freq_ID  = 7;
static const AkPluginParamID PARAM_Modular_AMP_ID   = 8;
static const AkPluginParamID PARAM_Carrier_Freq_ID  = 9;
static const AkPluginParamID PARAM_OSC_01_ID        = 10;
static const AkPluginParamID PARAM_OSC_02_ID        = 11;

static const AkUInt32 NUM_PARAMS = 12;

struct Voice_ModulatorRTPCParams
{
    AkReal32 fPlaceholder;
    AkReal32 fTreshold;
    AkReal32 fAattack;
    AkReal32 fRelease;
    AkReal32 fSat_Param;
    AkReal32 fFilter_Q;
    AkReal32 fFilter_Freq;
    AkReal32 fModular_Freq;
    AkReal32 fModular_AMP;
    AkReal32 fCarrier_Freq;
    AkReal32 fOSC_01;
    AkReal32 fOSC_02;
};

struct Voice_ModulatorNonRTPCParams
{
};

struct Voice_ModulatorFXParams
    : public AK::IAkPluginParam
{
    Voice_ModulatorFXParams();
    Voice_ModulatorFXParams(const Voice_ModulatorFXParams& in_rParams);

    ~Voice_ModulatorFXParams();

    /// Create a duplicate of the parameter node instance in its current state.
    IAkPluginParam* Clone(AK::IAkPluginMemAlloc* in_pAllocator) override;

    /// Initialize the plug-in parameter node interface.
    /// Initializes the internal parameter structure to default values or with the provided parameter block if it is valid.
    AKRESULT Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_ulBlockSize) override;

    /// Called by the sound engine when a parameter node is terminated.
    AKRESULT Term(AK::IAkPluginMemAlloc* in_pAllocator) override;

    /// Set all plug-in parameters at once using a parameter block.
    AKRESULT SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_ulBlockSize) override;

    /// Update a single parameter at a time and perform the necessary actions on the parameter changes.
    AKRESULT SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_ulParamSize) override;

    AK::AkFXParameterChangeHandler<NUM_PARAMS> m_paramChangeHandler;

    Voice_ModulatorRTPCParams RTPC;
    Voice_ModulatorNonRTPCParams NonRTPC;
};

#endif // Voice_ModulatorFXParams_H
