// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SandBoxTarget : TargetRules
{
	public SandBoxTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		bLegacyParentIncludePaths = false;
		CppStandard = CppStandardVersion.Cpp17;
		WindowsPlatform.bStrictConformanceMode = false;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "SandBox" } );
	}
}
