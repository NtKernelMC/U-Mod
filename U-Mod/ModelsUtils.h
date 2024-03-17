void LoadCustomModels();
bool IsModelExist(std::string model_name)
{
	for (const auto& it : CModelsList)
	{
		std::string mdl_name = std::get<0>(it.first);
		if (mdl_name.find(model_name) != std::string::npos) return true;
	}
	return false;
}
int SafeCast(std::string val)
{
	int cmd_val = 0;
	try
	{
		cmd_val = std::stoi(val);
	}
#pragma warning(suppress: 4101)
	catch (const std::invalid_argument& ia)
	{
		cmd_val = 0;
	}
#pragma warning(suppress: 4101)
	catch (const std::out_of_range& oor)
	{
		cmd_val = 0;
	}
	return cmd_val;
}
MODEL_TUPLE_1 GetFirstTuple(MODEL_CONTAINER container)
{
	for (const auto& it : container)
	{
		return it.first;
	}
	return std::make_tuple("", "");
}
MODEL_TUPLE_2 GetSecondTuple(MODEL_CONTAINER container)
{
	for (const auto& it : container)
	{
		return it.second;
	}
	return std::make_tuple(0, 0);
}
std::string ParseModelName(const std::string& input) 
{
	std::size_t startPos = input.find(": ");
	std::size_t endPos = input.find(" |");

	if (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos) 
	{
		startPos += 2; // Сдвигаем на два символа, чтобы пропустить ": "
		return input.substr(startPos, endPos - startPos);
	}

	return ""; // Возвращаем пустую строку, если не удалось найти имя модели
}
void ApplyModel(std::string mdl_name, std::string mtype)
{
	for (auto& it : CModelsList)
	{
		MODEL_TUPLE_1 tulpa = it.first;
		MODEL_TUPLE_2& tulpa_2 = it.second;
		if (findStringIC(std::get<0>(tulpa), mdl_name))
		{
			if (model_info.skin_id != 0u && findStringIC(mtype, xorstr_("Player")))
			{
				std::get<0>(tulpa_2) = model_info.skin_id;
				std::get<1>(tulpa_2) = 1;
				LogInFile(LOG_NAME, xorstr_("Applied skin param to model: %s\n"), mdl_name.c_str());
			}
			if (model_info.vehicle_id != 0u && findStringIC(mtype, xorstr_("Vehicle")))
			{
				std::get<0>(tulpa_2) = model_info.vehicle_id;
				std::get<1>(tulpa_2) = 2;
				LogInFile(LOG_NAME, xorstr_("Applied vehicle param to model: %s\n"), mdl_name.c_str());
			}
			if (model_info.weapon_id != 0u && findStringIC(mtype, xorstr_("Weapon")))
			{
				std::get<0>(tulpa_2) = model_info.weapon_id;
				std::get<1>(tulpa_2) = 3;
				LogInFile(LOG_NAME, xorstr_("Applied weapon param to model: %s\n"), mdl_name.c_str());
			}
			break;
		}
	}
}
void ApplyCustomModels(std::string selected_name, std::string mtype)
{
	std::string mdl_name = ParseModelName(selected_name);
	LogInFile(LOG_NAME, xorstr_("Applying model with name: %s\n"), mdl_name.c_str());
	DeleteFileA(xorstr_("U-Mod.dat"));
	FILE* hFile = fopen(xorstr_("U-Mod.dat"), xorstr_("a+"));
	if (hFile != nullptr)
	{
		ApplyModel(mdl_name, mtype);
		for (const auto& it : CModelsList)
		{
			MODEL_TUPLE_1 tpl_1 = it.first;
			MODEL_TUPLE_2 tpl_2 = it.second;
			std::string model_path = std::get<0>(tpl_1).c_str();
			DWORD model_id = std::get<0>(tpl_2);
			BYTE model_type = std::get<1>(tpl_2);
			fprintf(hFile, xorstr_("%s\n%lu\n%u\n"), model_path.c_str(), model_id, model_type);
		}
		fclose(hFile);
		LoadCustomModels();
	}
}
MODEL_CONTAINER ReadModelInfo(std::string cmodel_path)
{
	if (cmodel_path.empty()) cmodel_path = xorstr_("ReadModelInfo - empty path.");
	MODEL_CONTAINER tmp_model; BYTE modelType = 0;
	std::string model_path = cmodel_path; DWORD modelID = 0;
	std::ifstream infile(xorstr_("U-Mod.dat"));
	if (infile.is_open())
	{
		std::string line; UINT lineCounter = 1;
		while (std::getline(infile, line))
		{
			if (line.empty()) continue;
			switch (lineCounter)
			{
			case 1:
				model_path = line;
				break;
			case 2:
				modelID = SafeCast(line);
				break;
			case 3:
				modelType = SafeCast(line);
				if (cmodel_path.find(model_path) != std::string::npos)
				{
					MODEL_TUPLE_1 tpl_1 = std::make_tuple(model_path, "");
					MODEL_TUPLE_2 tpl_2 = std::make_tuple(modelID, modelType);
					tmp_model.insert(tmp_model.begin(), MODEL_PAIR(tpl_1, tpl_2));
					return tmp_model;
				}
				break;
			}
			lineCounter++;
			if (lineCounter > 3) lineCounter = 1;
		}
	}
	MODEL_TUPLE_1 tpl_1 = std::make_tuple(model_path, xorstr_("NONE"));
	MODEL_TUPLE_2 tpl_2 = std::make_tuple(modelID, modelType);
	tmp_model.insert(tmp_model.begin(), MODEL_PAIR(tpl_1, tpl_2));
	return tmp_model;
}
std::string GetShortName(std::string cmodel_path) 
{
	std::size_t pos = cmodel_path.find_last_of("\\");
	std::string filename;
	if (pos != std::string::npos) 
	{
		filename = cmodel_path.substr(pos + 1);
	}
	else filename = cmodel_path;

	// Удаление расширения .dff
	pos = filename.rfind(".dff");
	if (pos != std::string::npos && pos == (filename.length() - 4)) 
	{ // Проверяем, что .dff находится в конце строки
		filename = filename.substr(0, pos);
	}

	// Удаление расширения .txd
	pos = filename.rfind(".txd");
	if (pos != std::string::npos && pos == (filename.length() - 4)) 
	{ // Проверяем, что .txd находится в конце строки
		filename = filename.substr(0, pos);
	}

	return filename;
}
std::string MakeDrawableName(std::string cmodel_path, DWORD mdl_id, BYTE mdl_type)
{
	if (cmodel_path.empty()) return std::string(xorstr_("MakeDrawableName - EMPTY_PATH"));
	LogInFile(LOG_NAME, xorstr_("MakeDrawableName -> cmodel_path: %s | mdl_id: %d | mdl_type: %d\n"), cmodel_path.c_str(), mdl_id, mdl_type);
	char drawable_name[500]; memset(drawable_name, 0, sizeof(drawable_name));
	std::string replacing_id = "";
	if (mdl_id == 0u) replacing_id = xorstr_("Not selected");
	else replacing_id = std::to_string(mdl_id);
	std::string mdlType = "";
	if (mdl_type == 0u) mdlType = xorstr_("Not specified");
	else
	{
		if (mdl_type == 1) mdlType = xorstr_("Player");
		if (mdl_type == 2) mdlType = xorstr_("Vehicle");
		if (mdl_type == 3) mdlType = xorstr_("Weapon");
		if (mdl_type > 3) mdlType = xorstr_("Unknown");
	}
	std::string short_name = GetShortName(cmodel_path);
	snprintf(drawable_name, sizeof(drawable_name), xorstr_("Model Name: %s | Replacing ID: %s | Type: %s"), short_name.c_str(), replacing_id.c_str(), mdlType.c_str());
	return std::string(drawable_name);
}
void FillModelInfo(std::string cmodel_path)
{
	if (cmodel_path.empty())
	{
		LogInFile(LOG_NAME, xorstr_("Error: FillModelInfo - empty path.\n"));
		return;
	}
	if (!findStringIC(cmodel_path, xorstr_(".txd"))) // нет надобности парсить и .txd, мы просто допишем расширение вместо .dff
	{
		MODEL_CONTAINER mdl_info = ReadModelInfo(cmodel_path);
		MODEL_TUPLE_1 mdl_first = GetFirstTuple(mdl_info);
		MODEL_TUPLE_2 mdl_second = GetSecondTuple(mdl_info);
		std::get<1>(mdl_first) = MakeDrawableName(cmodel_path, std::get<0>(mdl_second), std::get<1>(mdl_second));
		CModelsList.insert(CModelsList.begin(), MODEL_PAIR(mdl_first, mdl_second));
		CEasyRegistry* reg = new CEasyRegistry(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\UKRAINEGTA: GLAB3\\Common", false);
		if (reg != nullptr)
		{
			std::string res_path = reg->ReadString("GTA:SA Path") + xorstr_("\\mods\\deathmatch\\resources\\fix_build\\");
			std::string short_name = GetShortName(cmodel_path);
			std::string dff_name = res_path + short_name + xorstr_(".dff");
			std::string txd_name = res_path + short_name + xorstr_(".txd");
			std::string orig_txd = "";
			CEasyRegistry* creg = new CEasyRegistry(HKEY_CURRENT_USER, xorstr_("Software\\SFWUM0D"), false);
			if (creg != nullptr)
			{
				// дописываем расширение и для .txd т.к мы спарсили только .dff файл но имя файла всегда одинаковое
				orig_txd = creg->ReadString(xorstr_("ModelsDir"));
				orig_txd += xorstr_("\\") + short_name + xorstr_(".txd");
				delete creg;
			}
			// копируем их в целевой ресурс луа инжектора
			BOOL rslt = CopyFileA(orig_txd.c_str(), txd_name.c_str(), FALSE);
			LogInFile(LOG_NAME, "[LOG] Copied file: %s to %s | %d\n", orig_txd.c_str(), txd_name.c_str(), (int)rslt);
			rslt = CopyFileA(cmodel_path.c_str(), dff_name.c_str(), FALSE);
			LogInFile(LOG_NAME, "[LOG] Copied file: %s to %s | %d\n", orig_txd.c_str(), txd_name.c_str(), (int)rslt);
			if (std::get<0>(mdl_second) != 0u)
			{
				lastModelID = std::get<0>(mdl_second);
				HackInjectAvailable = true;
				char cmd_str[256]; memset(cmd_str, 0, sizeof(cmd_str));
				sprintf(cmd_str, xorstr_("%d"), 1013);
				gLuaCode = cmd_str;
				gLuaArg = short_name;
				delete reg;
				Sleep(1000);
			}
		}
	}
}
void ParseModels(std::string cmodel_path, std::string extension)
{
	WIN32_FIND_DATAA findFileData;
	std::string prepare_path = cmodel_path + "\\";
	cmodel_path += extension;
	HANDLE MyHandle = FindFirstFileA(cmodel_path.c_str(), &findFileData);
	if (MyHandle != INVALID_HANDLE_VALUE)
	{
		LogInFile(LOG_NAME, xorstr_("ParseModels -> %s\n"), findFileData.cFileName);
		if (!IsModelExist(findFileData.cFileName)) FillModelInfo(prepare_path + findFileData.cFileName);
		while (FindNextFileA(MyHandle, &findFileData) != 0)
		{
			if (!IsModelExist(findFileData.cFileName)) FillModelInfo(prepare_path + findFileData.cFileName);
		}
	}
	FindClose(MyHandle);
}
void LoadCustomModels()
{
	CEasyRegistry* reg = new CEasyRegistry(HKEY_CURRENT_USER, xorstr_("Software\\SFWUM0D"), true);
	if (reg != nullptr)
	{
		std::string cmodel_path = reg->ReadString(xorstr_("ModelsDir"));
		LogInFile(LOG_NAME, xorstr_("LoadCustomModels -> ModelsDir: %s\n"), cmodel_path.c_str());
		if (!cmodel_path.empty() && cmodel_path.length() > 3)
		{
			CModelsList.clear();
			ParseModels(cmodel_path, xorstr_("\\*.txd"));
			ParseModels(cmodel_path, xorstr_("\\*.dff"));
		}
		delete reg;
	}
}