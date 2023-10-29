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

#include "Voice_ModulatorFXParams.h"

#include <AK/Tools/Common/AkBankReadHelpers.h>

Voice_ModulatorFXParams::Voice_ModulatorFXParams()
{
}

Voice_ModulatorFXParams::~Voice_ModulatorFXParams()
{
}

Voice_ModulatorFXParams::Voice_ModulatorFXParams(const Voice_ModulatorFXParams& in_rParams)
{
    RTPC = in_rParams.RTPC;
    NonRTPC = in_rParams.NonRTPC;
    m_paramChangeHandler.SetAllParamChanges();
}

AK::IAkPluginParam* Voice_ModulatorFXParams::Clone(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, Voice_ModulatorFXParams(*this));
}

AKRESULT Voice_ModulatorFXParams::Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    if (in_ulBlockSize == 0)
    {
        // Initialize default parameters here
        RTPC.fPlaceholder   = 0.0f;
        RTPC.fTreshold      = 0.7;
        RTPC.fAattack       = 0.01;
        RTPC.fRelease       = 0.001;
        RTPC.fSat_Param     = 0.001f;
        RTPC.fFilter_Q      = 0.001f;
        RTPC.fFilter_Freq   = 400.0f;
        RTPC.fModular_Freq  = 10.0f;
        RTPC.fModular_AMP   = 1.0f;
        RTPC.fCarrier_Freq  = 1.0f;
        RTPC.fOSC_01        = 0;
        RTPC.fOSC_02        = 0;
        m_paramChangeHandler.SetAllParamChanges();
        return AK_Success;
    }

    return SetParamsBlock(in_pParamsBlock, in_ulBlockSize);
}

AKRESULT Voice_ModulatorFXParams::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT Voice_ModulatorFXParams::SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    AKRESULT eResult = AK_Success;
    AkUInt8* pParamsBlock = (AkUInt8*)in_pParamsBlock;

    // Read bank data here
    RTPC.fPlaceholder = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fTreshold = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fAattack = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fRelease = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fSat_Param = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fFilter_Q = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fFilter_Freq = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fModular_Freq = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fModular_AMP = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fCarrier_Freq = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fOSC_01 = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fOSC_02 = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    CHECKBANKDATASIZE(in_ulBlockSize, eResult);
    m_paramChangeHandler.SetAllParamChanges();

    return eResult;
}

AKRESULT Voice_ModulatorFXParams::SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_ulParamSize)
{
    AKRESULT eResult = AK_Success;

    // Handle parameter change here
    switch (in_paramID)
    {
    case PARAM_PLACEHOLDER_ID:
        RTPC.fPlaceholder = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_PLACEHOLDER_ID);
        break;
    case PARAM_Treshold_ID:
        RTPC.fTreshold = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Treshold_ID);
        break;
    case PARAM_Aattack_ID:
        RTPC.fAattack = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Aattack_ID);
        break;
    case PARAM_Release_ID:
        RTPC.fRelease = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Release_ID);
        break;
    case PARAM_Sat_Param_ID:
        RTPC.fSat_Param = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Sat_Param_ID);
        break;
    case PARAM_Filter_Q_ID:
        RTPC.fFilter_Q = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Filter_Q_ID);
        break;
    case PARAM_Filter_Freq_ID:
        RTPC.fFilter_Freq = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Filter_Freq_ID);
        break;
    case PARAM_Modular_Freq_ID:
        RTPC.fModular_Freq = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Modular_Freq_ID);
        break;
    case PARAM_Modular_AMP_ID:
        RTPC.fModular_AMP = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Modular_AMP_ID);
        break;
    case PARAM_Carrier_Freq_ID:
        RTPC.fCarrier_Freq = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_Carrier_Freq_ID);
        break;
    case PARAM_OSC_01_ID:
        RTPC.fOSC_01 = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_01_ID);
        break;
    case PARAM_OSC_02_ID:
        RTPC.fOSC_02 = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSC_02_ID);
        break;
    default:
        eResult = AK_InvalidParameter;
        break;
    }

    return eResult;
}
