#include "Plugin.h"

const string PluginBase::plaintextSeparator = ": ";

string PluginBase::getJson()
{
	if (jsonCache.empty())
	{
		for (int i = 0; i < (int)sections.size(); i++)
		{
			if (i)
				jsonCache += ",";
			
			jsonCache += "{";
			
			jsonCache += "\"full_text\":\"" + sections[i].text + "\"";
			
			if (!sections[i].color.empty())
			{
				jsonCache += ",\"color\":\"" + sections[i].color + "\"";
			}
			
			if (!sections[i].bknd.empty())
			{
				jsonCache += ",\"background\":\"" + sections[i].bknd + "\"";
			}
			
			//if (i != (int)sections.size() - 1)
			{
				jsonCache += ",\"separator\":false,\"separator_block_width\":0";
			}
			/*else
			{
				jsonCache += ",\"separator\":true";
			}*/
			
			jsonCache += "}";
		}
		
		if (jsonCache.empty())
		{
			jsonCache =  "{\"full_text\":\"[empty section]\"}";
		}
	}
	
	return jsonCache;
}

string PluginBase::getPlaintext()
{
	if (plaintextCache.empty())
	{
		for (int i = 0; i < (int)sections.size(); i++)
		{
			if (i)
				plaintextCache += plaintextSeparator;
			
			plaintextCache += sections[i].text;
		}
	}
	
	return plaintextCache;
}

double PluginBase::update(double delta)
{
	jsonCache = "";
	plaintextCache = "";
	
	sections.clear();
	
	return refresh(delta);
}

Plugin labelPlugin(PluginBase::Section in);
Plugin labelPlugin(ConfigData * config);
Plugin cpuPlugin(ConfigData * config);
Plugin ramPlugin(ConfigData * config);
Plugin timePlugin(ConfigData * config);

Plugin PluginBase::make(ConfigData * config)
{
	string type = config->get("type").asString("missing");
	
	if (type == "missing")
	{
		return labelPlugin(PluginBase::Section("[section without type]", "#ffffff", "#ff0000"));
	}
	else if (type == "label")
	{
		return labelPlugin(config);
	}
	else if (type == "cpu")
	{
		return cpuPlugin(config);
	}
	else if (type == "ram")
	{
		return ramPlugin(config);
	}
	else if (type == "time")
	{
		return timePlugin(config);
	}
			
		//case "ram":
			
		//case "time":
			
	else
	{
		return labelPlugin(PluginBase::Section("[section with unknown type '" + type + "']", "#ffffff", "#ff0000"));
	}
}

Plugin PluginBase::make(string text, string color, string bknd)
{
	return labelPlugin(PluginBase::Section(text, color, bknd));
}

std::function<string(double)> getDoubleToStringFunc(ConfigData * config)
{
	string style = config->get("style").asString();
	
	if (style.empty() || style == "percent")
	{
		return doubleAsPercent;
	}
	else if (style == "v_bar")
	{
		return verticalBar;
	}
	else if (style == "pie")
	{
		return pieChart;
	}
	else if (style == "dot")
	{
		return dot;
	}
	else if (style == "h_bar")
	{
		int w = config->get("bar_size").asNum();
		
		if (w == 0)
			w = 1;
		
		return [=](double in) -> string {return horizontalBar(in, w);};
	}
	else
	{
		return [=](double in) -> string {return "unknown style '" + style + "'";};
	}
}

