#include "statusline.h"
#include <unistd.h>

#include "graphics.h"

#include "Plugin.h"

#include "file_utils.h"

ConfigData globalConfig;
vector<ConfigData> itemsConfig;
vector<Plugin> plugins;
Plugin separator = PluginBase::make(" │ ", "#ffffff");

string configPath=getHomeDir()+"/.config/wmww_status.conf";

void loadConfig();

// returns true if program should abort
bool processArgs(int argc, char ** argv)
{
	vector<string> args;
	bool quit=false;
	
	for (int i=1; i<argc; i++)
	{
		args.push_back(string(argv[i]));
	}
	
	for (int i=0; i<(int)args.size(); i++)
	{
		if (args[i] == "-h")
		{
			cout << "WMWW Statusline" << endl;
			cout << "more info at https://github.com/william01110111/wmwwStatusline" << endl;
			cout << "usage: wmww_status [OPTIONS]" << endl;
			cout << "options:" << endl;
			cout << "    -c, -config [config filepath] - specify a config filepath" << endl;
			cout << "    -show-parsed - show the pased config gile and quit" << endl;
			quit = true;
			break;
		}
		else if (args[i] == "-c" || args[i] == "-config")
		{
			if (i + 1 < (int)args.size())
			{
				configPath = args[i+1];
				i++;
			}
		}
		else if (args[i] == "-show-parsed")
		{
			loadConfig();
			cout << "parsed config file:" << endl;
			cout << globalConfig.toString() << endl;
			for (auto j: itemsConfig)
			{
				cout << "{\n" << j.toString() << "}" << endl << endl;
			}
			quit = true;
			break;
		}
		else
		{
			cout << "ERROR: unrecognized option '" << args[i] << "'" << endl;
			cout << "run 'wmww_status -h' for help" << endl;
			quit = true;
			break;
		}
	}
	
	return quit;
}

void loadConfig()
{
	globalConfig.clear();
	itemsConfig.clear();
	plugins.clear();
	
	if (!globalConfig.fromFile(configPath, itemsConfig))
	{
		globalConfig.fromString("{type: label; text: \"no config file at '" + configPath + "'\"}", itemsConfig);
	}
	
	if (globalConfig.get("live_reload").exists())
		plugins.push_back(PluginBase::make("live reload active", "#000000", "#ff8000"));
	
	for (auto i: itemsConfig)
	{
		if (i.get("active").asString() != "false")
			plugins.push_back(PluginBase::make(&i));
	}
	
	//cout << globalConfig.toString() << endl;
}

int main(int argc, char ** argv)
{
	if (processArgs(argc, argv))
		return 0;
	
	loadConfig();
	
	bool useJson = globalConfig.get("use_json").exists();
	
	if (useJson)
	{
		cout << "{\"version\":1}" << endl << "[" << endl;
	}
	
	while (true)
	{
		if (globalConfig.get("live_reload").exists())
			loadConfig();
		
		bool isStart = true;
		string out;
		
		for (auto i: plugins)
		{
			i->update(1);
			
			if (useJson)
			{
				if (!isStart)
					out += "," + separator->getJson() + ",";
				isStart = false;
				
				out += i->getJson();
			}
			else
			{
				if (!isStart)
					out += separator->getPlaintext();
				isStart = false;
				out += i->getPlaintext();
			}
		}
		
		if (useJson)
		{
			out = "[" + out + "],";
		}
		
		cout << out << endl;
		usleep(600000);
	}
	
	/*while (true)
	{
		usleep(1000000);
		cout << getCpu() << endl;
		cout << getRam() << endl;
	}*/
	
	/*
	PersistentData data;
	
	if (!data.loadFrom("config.txt", "config"))
	{
		cout << "error loading file" << endl;
	}
	*/
	
	//cout << data.toString() << endl;
	
	//cout << getCpu() << endl;
	
	//cout << *((int*)0) << endl;
	
	/*while (true)
	{
		cout << "RAM: " << verticalBar(getRam()) << " | " << "CPU: " << verticalBar(getCpu()/100.0) << " | " << getTime() << endl;
		usleep(600000);
	}*/
	
	return 0;
}

