using UnrealBuildTool;

public class SingularisFirstPerson : ModuleRules
{
	public SingularisFirstPerson(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",
				"Engine",

				"InputCore",
				"EnhancedInput",
			]
		);
	}
}