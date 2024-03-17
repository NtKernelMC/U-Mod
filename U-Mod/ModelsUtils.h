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
	std::size_t pos = cmodel_path.find_last_of(xorstr_("\\"));
	if (pos != std::string::npos)
	{
		return cmodel_path.substr(pos + 1);
	}
	return cmodel_path;
}
std::string MakeDrawableName(std::string cmodel_path, DWORD mdl_id, BYTE mdl_type)
{
	if (cmodel_path.empty()) return std::string(xorstr_("MakeDrawableName - EMPTY_PATH"));
	LogInFile(LOG_NAME, xorstr_("MakeDrawableName -> cmodel_path: %s | mdl_id: %d | mdl_type: %d\n"), cmodel_path.c_str(), mdl_id, mdl_type);
	char drawable_name[500]; memset(drawable_name, 0, sizeof(drawable_name));
	std::string replacing_id = "";
	if (mdl_id == 0u) replacing_id = xorstr_("Не выбран");
	else replacing_id = std::to_string(mdl_id);
	std::string mdlType = "";
	if (mdl_type == 0u) mdlType = xorstr_("Не указан");
	else
	{
		if (mdl_type == 1) mdlType = xorstr_("Скин игрока");
		if (mdl_type == 2) mdlType = xorstr_("Модель автомобиля");
		if (mdl_type == 3) mdlType = xorstr_("Модель оружия");
		if (mdl_type > 3) mdlType = xorstr_("Неизвестен");
	}
	std::string short_name = GetShortName(cmodel_path);
	snprintf(drawable_name, sizeof(drawable_name), xorstr_("%s | Заменяет ID: %s | Тип: %s"), short_name.c_str(), replacing_id.c_str(), mdlType.c_str());
	return std::string(drawable_name);
}
void FillModelInfo(std::string cmodel_path)
{
	MODEL_CONTAINER mdl_info = ReadModelInfo(cmodel_path);
	MODEL_TUPLE_1 mdl_first = GetFirstTuple(mdl_info);
	MODEL_TUPLE_2 mdl_second = GetSecondTuple(mdl_info);
	std::get<1>(mdl_first) = MakeDrawableName(cmodel_path, std::get<0>(mdl_second), std::get<1>(mdl_second));
	CModelsList.insert(CModelsList.begin(), MODEL_PAIR(mdl_first, mdl_second));
}
void ParseModels(std::string cmodel_path, std::string extension)
{
	WIN32_FIND_DATAA findFileData;
	std::string prepare_path = cmodel_path + "\\";
	cmodel_path += extension;
	HANDLE MyHandle = FindFirstFileA(cmodel_path.c_str(), &findFileData);
	if (MyHandle != INVALID_HANDLE_VALUE)
	{
		CModelsList.clear();
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
			ParseModels(cmodel_path, xorstr_("\\*.txd"));
			ParseModels(cmodel_path, xorstr_("\\*.dff"));
		}
		delete reg;
	}
}