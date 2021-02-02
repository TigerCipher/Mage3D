/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: team@bluemoondev.org
 * 
 * File Name: ScriptCommander.cpp
 * Date File Created: 2/1/2021 at 2:46 PM
 * Author: Matt
 */


#include "ScriptCommander.h"
#include "TextureProcessor.h"

#include <nlohmann/json.hpp>
#include <fstream>


namespace js = nlohmann;

#define CMD_ERR(msg) ParseException(__LINE__, __FILE__, path, (msg))

ScriptCommander::ScriptCommander(const list<std::string>& args)
{
	if(args.size() >= 2 && args[0] == "--commands")
	{
		const auto path = args[1];
		std::ifstream script(path);
		if (!script.is_open())
			throw CMD_ERR("Failed to open file");

		js::json top;
		script >> top;

		
		if(top.at("enabled"))
		{
			bool abort = false;
			
			for(const auto& j : top.at("commands"))
			{
				const auto cmdName = j.at("command").get<std::string>();
				const auto params = j.at("params");

				if(cmdName == "flipY")
				{
					const auto src = params.at("source");
					TextureProcessor::flipYNormalMap(src, params.value("dest", src));
					abort = true;
				}else if(cmdName == "flipAllY")
				{
					TextureProcessor::flipYForAllModelNormalMaps(params.at("source"));
					abort = true;
				}else if(cmdName == "validate-normals")
				{
					TextureProcessor::validateNormalMap(
						params.at("source"), 
						params.at("min"), 
						params.at("max"));
					abort = true;
				}else if(cmdName == "reformatAll")
				{
					TextureProcessor::reformatAllTextures(params.at("model"));
					abort = true;
				}else if(cmdName == "reformat")
				{
					TextureProcessor::reformatTexture(params.at("source"));
					abort = true;
				}
				else
					throw CMD_ERR("Unknown command: " + cmdName);
			}
			if (abort) throw SuccessException("Successfully executed all commands");
		}
	}
}

char const* ScriptCommander::ParseException::what() const noexcept
{
	mWhat = fmt::format("{}\n[Script File] {}\n{}", getType(), mCommand, mMessage);
	return mWhat.c_str();
}

char const* ScriptCommander::SuccessException::what() const noexcept
{
	mWhat = "Command execution completed\n\n" + mContent;
	return mWhat.c_str();
}
