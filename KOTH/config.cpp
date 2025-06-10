class CfgPatches
{
	class KOTH
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
			"LBmaster_Groups"
		};
	};
};
class CfgMods
{
	class KOTH_Mod
	{
		dir="KOTH";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="KOTH";
		credits="MDC";
		author="MDC";
		authorID="0";
		version="1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"World",
			"Game",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"KOTH/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"KOTH/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"KOTH/Scripts/5_Mission"
				};
			};
		};
	};
};
