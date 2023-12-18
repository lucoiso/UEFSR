//------------------------------------------------------------------------------
// FidelityFX Super Resolution UE4 Plugin
//
// Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//------------------------------------------------------------------------------
#include "FSRSubpassFirst.h"

#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 2)
#include <SceneRendering.h>
#endif

void FFSRSubpassFirst::CreateResources(FRDGBuilder& GraphBuilder, const FViewInfo& View, const FInputs& PassInputs)
{
	Data->FSROutputTextureDesc = PassInputs.SceneColor.Texture->Desc;
	Data->FSROutputTextureDesc.Reset();
	Data->FSROutputTextureDesc.Extent = View.UnscaledViewRect.Max;
	Data->FSROutputTextureDesc.ClearValue = FClearValueBinding::Black;
	Data->FSROutputTextureDesc.Flags = TexCreate_ShaderResource | TexCreate_UAV | TexCreate_RenderTargetable;
}

void FFSRSubpassFirst::Upscale(FRDGBuilder& GraphBuilder, const FViewInfo& View, const FInputs& PassInputs)
{
	Data->FinalOutput = PassInputs.SceneColor; // later subpasses will override this, if enabled
}

void FFSRSubpassFirst::PostProcess(FRDGBuilder& GraphBuilder, const FViewInfo& View, const FInputs& PassInputs)
{
	// if we never ran the Upscale pass, make sure we get initialized before running the PostProcess pass
	if (!Data->FinalOutput.IsValid())
	{
		check(PassInputs.OverrideOutput.IsValid());
		Data->FinalOutput = PassInputs.OverrideOutput; // later subpasses will override this, if enabled
	}
}