// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SandBoxEditorTarget : TargetRules
{
	public SandBoxEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		bLegacyParentIncludePaths = false;
		CppStandard = CppStandardVersion.Cpp17;
		WindowsPlatform.bStrictConformanceMode = false;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "SandBox" } );
	}
}
