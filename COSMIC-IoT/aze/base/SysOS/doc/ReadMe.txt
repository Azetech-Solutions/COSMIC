Folder Structure
----------------
SysOs
	|
	|---- api
	|
	|---- cfg
	|
	|---- doc
	|
	|---- scripts
	|		|
	|		|---- schema
	|		|
	|		|---- templates
	|
	|---- src

1. scripts\schema folder will have all the schema definition for the SysOs.
2. scripts\templates folder will have the scripts to be generated as a code file.
3. cfg folder will have the EasyConfig configuration file and generated code for compilation.

Integration Manual:
-------------------
1. SysOs_Config.xml shall be the ONLY file where a task shall be created scheduled. There should not be any project specific settings.
2. For each task being scheduled in SysOs, there shall be dedicated ProcessSection and ProcessGroup definition settings.
3. SubSystem definition shall be present in every components of the project.

