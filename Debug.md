Add these files in `.vscode` folder

## `tasks.json`
```json
{
	"version": "2.0.0",
	"tasks": [
		{
			"label": "Make",
			"type": "shell",
			"command": "",
			"args": [
				"make",
				"debug"
			],
			"problemMatcher": [
				"$gcc"
			],
			"group": {
				"kind": "build",
				"isDefault": true
			},
			"detail": "compiler: /usr/bin/g++"
		}
	]
}
```

## `launch.json`
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/Build/parser.out",
            "args": ["${workspaceFolder}/Build/test.nb"],
            "stopAtEntry": true,
            "cwd": "${workspaceFolder}/Build",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "preLaunchTask": "Make",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                },
                {
                    "description": "Set Disassembly Flavor to Intel",
                    "text": "-gdb-set disassembly-flavor intel",
                    "ignoreFailures": true
                }
            ]
        }

    ]
}
```