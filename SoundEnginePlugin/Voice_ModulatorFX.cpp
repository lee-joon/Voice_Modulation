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

#include "Voice_ModulatorFX.h"
#include "../Voice_ModulatorConfig.h"

#include <AK/AkWwiseSDKVersion.h>

AK::IAkPlugin* CreateVoice_ModulatorFX(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, Voice_ModulatorFX());
}

AK::IAkPluginParam* CreateVoice_ModulatorFXParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, Voice_ModulatorFXParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(Voice_ModulatorFX, AkPluginTypeEffect, Voice_ModulatorConfig::CompanyID, Voice_ModulatorConfig::PluginID)

Voice_ModulatorFX::Voice_ModulatorFX()
    : m_pParams(nullptr)
    , m_pAllocator(nullptr)
    , m_pContext(nullptr)
{
}

Voice_ModulatorFX::~Voice_ModulatorFX()
{
}

AKRESULT Voice_ModulatorFX::Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkEffectPluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat)
{
    m_pParams = (Voice_ModulatorFXParams*)in_pParams;
    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;
    m_PreRMS.resize(in_rFormat.channelConfig.uNumChannels, 0.0f);
    m_ResultAmp.resize(in_rFormat.channelConfig.uNumChannels, 0.0f);

    Filters::SampleRate = static_cast<AkReal32>(in_rFormat.uSampleRate);

    Filter.CutoffFrequence = m_pParams->RTPC.fFilter_Freq;
    Filter.Q = m_pParams->RTPC.fFilter_Q;

    m_SampleRate = in_rFormat.uSampleRate;


    return AK_Success;
}

AKRESULT Voice_ModulatorFX::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT Voice_ModulatorFX::Reset()
{
    return AK_Success;
}

AKRESULT Voice_ModulatorFX::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeEffect;
    out_rPluginInfo.bIsInPlace = true;
	out_rPluginInfo.bCanProcessObjects = false;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;
}

void Voice_ModulatorFX::Execute(AkAudioBuffer* io_pBuffer)
{
    const AkUInt32 uNumChannels = io_pBuffer->NumChannels();

    AkUInt16 uFramesProcessed;
    for (AkUInt32 i = 0; i < uNumChannels; ++i)
    {
        AkReal32* AK_RESTRICT pBuf = (AkReal32* AK_RESTRICT)io_pBuffer->GetChannel(i);

        uFramesProcessed = 0;
        AkUInt32 Timer = m_Timer;
        while (uFramesProcessed < io_pBuffer->uValidFrames)
        {
            m_PreRMS[i] = sqrtf((1.0f - Average) * m_PreRMS[i] + Average * pBuf[uFramesProcessed] * pBuf[uFramesProcessed]);
            AkReal32 CalculatorAmp = m_PreRMS[i] * m_pParams->RTPC.fModular_AMP;

            //AkReal32 Coeff = m_PreRMS[i] < m_pParams->RTPC.fTreshold ? m_pParams->RTPC.fRelease : m_pParams->RTPC.fAattack;
            //m_ResultAmp[i] = (1.0f - Coeff) * m_ResultAmp[i] + Coeff * CalculatorAmp;

            if (m_PreRMS[i] < m_pParams->RTPC.fTreshold)
            {
                AkReal32 Coeff = m_pParams->RTPC.fRelease;                                                          
                m_ResultAmp[i] =  Coeff * m_ResultAmp[i];
            }
            else
            {
                AkReal32 Coeff = m_pParams->RTPC.fAattack;
                m_ResultAmp[i] = (1.0f - Coeff) * m_ResultAmp[i] + Coeff * CalculatorAmp;
            }
            
            //FM Synth
            AkReal32 Muldular =  (LookupTable.Oscillator(static_cast<AkUInt32>(m_pParams->RTPC.fOSC_01), fmodf(Timer * m_pParams->RTPC.fModular_Freq /m_SampleRate, 1.0f)) + 1.0f) * 0.5f * m_ResultAmp[i];//Modular Synth
            //AkReal32 Muldular =  (LookupTable.Oscillator(static_cast<AkUInt32>(m_pParams->RTPC.fOSC_01), fmodf(Timer * m_pParams->RTPC.fModular_Freq / m_SampleRate, 1.0f)) + 1.0f) * 0.5f * m_pParams->RTPC.fModular_AMP;//Modular Synth
            AkReal32 FmSynth = LookupTable.Oscillator(static_cast<AkUInt32>(m_pParams->RTPC.fOSC_02), fmodf((m_pParams->RTPC.fCarrier_Freq * Timer ) / m_SampleRate + Muldular, 1.0f));

            pBuf[uFramesProcessed] = Filter.LowpassFilter(Saturate.SaturateFunc(pBuf[uFramesProcessed] * FmSynth, m_pParams->RTPC.fSat_Param));
            //pBuf[uFramesProcessed] = pBuf[uFramesProcessed] * FmSynth;
            
            ++Timer;
            ++uFramesProcessed;
        }
    }
    m_Timer = m_Timer + io_pBuffer->uValidFrames;
}

AKRESULT Voice_ModulatorFX::TimeSkip(AkUInt32 in_uFrames)
{
    return AK_DataReady;
}
