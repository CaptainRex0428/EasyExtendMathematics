// Some copyright should be here...

using UnrealBuildTool;

public class EasyExtendMathematics : ModuleRules
{
	public EasyExtendMathematics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		System.Console.WriteLine("\n========== Path Information ==========");
		System.Console.WriteLine("ModuleDirectory: " + ModuleDirectory);
		System.Console.WriteLine("PluginDirectory: " + PluginDirectory);
		
		string SourceDir = System.IO.Path.Combine(PluginDirectory, "Source");
		if (System.IO.Directory.Exists(SourceDir))
		{
			System.Console.WriteLine("\nSource directory contents:");
			foreach (var dir in System.IO.Directory.GetDirectories(SourceDir))
			{
				System.Console.WriteLine("  - " + System.IO.Path.GetFileName(dir));
			}
		}
		System.Console.WriteLine("======================================\n");
		
		PublicIncludePaths.AddRange(
			new string[] {
				// System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "/Shaders"
				// System.IO.Path.Combine(ModuleDirectory, "EasyMath/include"),
				// 或者使用插件根目录
				System.IO.Path.Combine(PluginDirectory, "Source/EasyMath/include")
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"RenderCore",
				"Renderer",
				"RHI",
				"UnrealEd",
				"MaterialEditor",
				"EditorStyle",
				"EditorWidgets",
				"ToolMenus",
				"Slate",
				"SlateCore",
				"MaterialUtilities",
				"RenderCore",
				"Renderer",
				
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Projects",
				"KismetCompiler",
				"GraphEditor",
				"BlueprintGraph",
				"KismetWidgets",
				"PropertyEditor",
				"TargetPlatform",
				"DeveloperSettings",
				"Projects",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
