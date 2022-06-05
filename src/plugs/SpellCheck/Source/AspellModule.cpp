/**
 *		SpellChecker for AkelPad
 *		Panych Y.W. aka FeyFre (c) 2010-2014 (panych.y@gmail.com)
 *
 *		This piece of software is distributed under BSD license.
 *		AkelPad is property of its copyright holders.
 *		The New Aspell Copyright (C) 2001-2002 by Kevin Atkinson under the GNU LGPL
 *		license version 2.0 or 2.1.
 */

#include "AspellModule.h"

CAspellModule* CAspellModule::theModule;
CAspellModule::CAspellModule(LPCWSTR path)
{
	hInst=LoadLibraryW(path);
	if(hInst)
	{
		//aspell_mutable_container_add=(PFaspell_mutable_container_add)GetProcAddress(hInst,Naspell_mutable_container_add);
		//aspell_mutable_container_remove=(PFaspell_mutable_container_remove)GetProcAddress(hInst,Naspell_mutable_container_remove);
		//aspell_mutable_container_clear=(PFaspell_mutable_container_clear)GetProcAddress(hInst,Naspell_mutable_container_clear);
		//aspell_mutable_container_to_mutable_container=(PFaspell_mutable_container_to_mutable_container)GetProcAddress(hInst,Naspell_mutable_container_to_mutable_container);
		//aspell_key_info_enumeration_at_end=(PFaspell_key_info_enumeration_at_end)GetProcAddress(hInst,Naspell_key_info_enumeration_at_end);
		//aspell_key_info_enumeration_next=(PFaspell_key_info_enumeration_next)GetProcAddress(hInst,Naspell_key_info_enumeration_next);
		//delete_aspell_key_info_enumeration=(PFdelete_aspell_key_info_enumeration)GetProcAddress(hInst,Ndelete_aspell_key_info_enumeration);
		//aspell_key_info_enumeration_clone=(PFaspell_key_info_enumeration_clone)GetProcAddress(hInst,Naspell_key_info_enumeration_clone);
		//aspell_key_info_enumeration_assign=(PFaspell_key_info_enumeration_assign)GetProcAddress(hInst,Naspell_key_info_enumeration_assign);
		new_aspell_config=(PFnew_aspell_config)GetProcAddress(hInst,Nnew_aspell_config);
		delete_aspell_config=(PFdelete_aspell_config)GetProcAddress(hInst,Ndelete_aspell_config);
		//aspell_config_clone=(PFaspell_config_clone)GetProcAddress(hInst,Naspell_config_clone);
		//aspell_config_assign=(PFaspell_config_assign)GetProcAddress(hInst,Naspell_config_assign);
		//aspell_config_error_number=(PFaspell_config_error_number)GetProcAddress(hInst,Naspell_config_error_number);
		//aspell_config_error_message=(PFaspell_config_error_message)GetProcAddress(hInst,Naspell_config_error_message);
		//aspell_config_error=(PFaspell_config_error)GetProcAddress(hInst,Naspell_config_error);
		//aspell_config_set_extra=(PFaspell_config_set_extra)GetProcAddress(hInst,Naspell_config_set_extra);
		//aspell_config_keyinfo=(PFaspell_config_keyinfo)GetProcAddress(hInst,Naspell_config_keyinfo);
		//aspell_config_possible_elements=(PFaspell_config_possible_elements)GetProcAddress(hInst,Naspell_config_possible_elements);
		//aspell_config_get_default=(PFaspell_config_get_default)GetProcAddress(hInst,Naspell_config_get_default);
		//aspell_config_elements=(PFaspell_config_elements)GetProcAddress(hInst,Naspell_config_elements);
		aspell_config_replace=(PFaspell_config_replace)GetProcAddress(hInst,Naspell_config_replace);
		//aspell_config_remove=(PFaspell_config_remove)GetProcAddress(hInst,Naspell_config_remove);
		//aspell_config_have=(PFaspell_config_have)GetProcAddress(hInst,Naspell_config_have);
		//aspell_config_retrieve=(PFaspell_config_retrieve)GetProcAddress(hInst,Naspell_config_retrieve);
		//aspell_config_retrieve_list=(PFaspell_config_retrieve_list)GetProcAddress(hInst,Naspell_config_retrieve_list);
		//aspell_config_retrieve_bool=(PFaspell_config_retrieve_bool)GetProcAddress(hInst,Naspell_config_retrieve_bool);
		//aspell_config_retrieve_int=(PFaspell_config_retrieve_int)GetProcAddress(hInst,Naspell_config_retrieve_int);
		//aspell_error_is_a=(PFaspell_error_is_a)GetProcAddress(hInst,Naspell_error_is_a);
		//aspell_error_number=(PFaspell_error_number)GetProcAddress(hInst,Naspell_error_number);
		aspell_error_message=(PFaspell_error_message)GetProcAddress(hInst,Naspell_error_message);
		aspell_error=(PFaspell_error)GetProcAddress(hInst,Naspell_error);
		delete_aspell_can_have_error=(PFdelete_aspell_can_have_error)GetProcAddress(hInst,Ndelete_aspell_can_have_error);
		new_aspell_speller=(PFnew_aspell_speller)GetProcAddress(hInst,Nnew_aspell_speller);
		to_aspell_speller=(PFto_aspell_speller)GetProcAddress(hInst,Nto_aspell_speller);
		delete_aspell_speller=(PFdelete_aspell_speller)GetProcAddress(hInst,Ndelete_aspell_speller);
		//aspell_speller_error_number=(PFaspell_speller_error_number)GetProcAddress(hInst,Naspell_speller_error_number);
		aspell_speller_error_message=(PFaspell_speller_error_message)GetProcAddress(hInst,Naspell_speller_error_message);
		//aspell_speller_error=(PFaspell_speller_error)GetProcAddress(hInst,Naspell_speller_error);
		aspell_speller_config=(PFaspell_speller_config)GetProcAddress(hInst,Naspell_speller_config);
		aspell_speller_check=(PFaspell_speller_check)GetProcAddress(hInst,Naspell_speller_check);
		//aspell_speller_add_to_personal=(PFaspell_speller_add_to_personal)GetProcAddress(hInst,Naspell_speller_add_to_personal);
		//aspell_speller_add_to_session=(PFaspell_speller_add_to_session)GetProcAddress(hInst,Naspell_speller_add_to_session);
		//aspell_speller_personal_word_list=(PFaspell_speller_personal_word_list)GetProcAddress(hInst,Naspell_speller_personal_word_list);
		//aspell_speller_session_word_list=(PFaspell_speller_session_word_list)GetProcAddress(hInst,Naspell_speller_session_word_list);
		//aspell_speller_main_word_list=(PFaspell_speller_main_word_list)GetProcAddress(hInst,Naspell_speller_main_word_list);
		//aspell_speller_save_all_word_lists=(PFaspell_speller_save_all_word_lists)GetProcAddress(hInst,Naspell_speller_save_all_word_lists);
		//aspell_speller_clear_session=(PFaspell_speller_clear_session)GetProcAddress(hInst,Naspell_speller_clear_session);
		aspell_speller_suggest=(PFaspell_speller_suggest)GetProcAddress(hInst,Naspell_speller_suggest);
		//aspell_speller_store_replacement=(PFaspell_speller_store_replacement)GetProcAddress(hInst,Naspell_speller_store_replacement);
		//delete_aspell_filter=(PFdelete_aspell_filter)GetProcAddress(hInst,Ndelete_aspell_filter);
		//aspell_filter_error_number=(PFaspell_filter_error_number)GetProcAddress(hInst,Naspell_filter_error_number);
		//aspell_filter_error_message=(PFaspell_filter_error_message)GetProcAddress(hInst,Naspell_filter_error_message);
		//aspell_filter_error=(PFaspell_filter_error)GetProcAddress(hInst,Naspell_filter_error);
		//to_aspell_filter=(PFto_aspell_filter)GetProcAddress(hInst,Nto_aspell_filter);
		//delete_aspell_document_checker=(PFdelete_aspell_document_checker)GetProcAddress(hInst,Ndelete_aspell_document_checker);
		//aspell_document_checker_error_number=(PFaspell_document_checker_error_number)GetProcAddress(hInst,Naspell_document_checker_error_number);
		//aspell_document_checker_error_message=(PFaspell_document_checker_error_message)GetProcAddress(hInst,Naspell_document_checker_error_message);
		//aspell_document_checker_error=(PFaspell_document_checker_error)GetProcAddress(hInst,Naspell_document_checker_error);
		//new_aspell_document_checker=(PFnew_aspell_document_checker)GetProcAddress(hInst,Nnew_aspell_document_checker);
		//to_aspell_document_checker=(PFto_aspell_document_checker)GetProcAddress(hInst,Nto_aspell_document_checker);
		//aspell_document_checker_reset=(PFaspell_document_checker_reset)GetProcAddress(hInst,Naspell_document_checker_reset);
		//aspell_document_checker_process=(PFaspell_document_checker_process)GetProcAddress(hInst,Naspell_document_checker_process);
		//aspell_document_checker_next_misspelling=(PFaspell_document_checker_next_misspelling)GetProcAddress(hInst,Naspell_document_checker_next_misspelling);
		//aspell_document_checker_filter=(PFaspell_document_checker_filter)GetProcAddress(hInst,Naspell_document_checker_filter);
		//aspell_word_list_empty=(PFaspell_word_list_empty)GetProcAddress(hInst,Naspell_word_list_empty);
		//aspell_word_list_size=(PFaspell_word_list_size)GetProcAddress(hInst,Naspell_word_list_size);
		aspell_word_list_elements=(PFaspell_word_list_elements)GetProcAddress(hInst,Naspell_word_list_elements);
		delete_aspell_string_enumeration=(PFdelete_aspell_string_enumeration)GetProcAddress(hInst,Ndelete_aspell_string_enumeration);
		//aspell_string_enumeration_clone=(PFaspell_string_enumeration_clone)GetProcAddress(hInst,Naspell_string_enumeration_clone);
		//aspell_string_enumeration_assign=(PFaspell_string_enumeration_assign)GetProcAddress(hInst,Naspell_string_enumeration_assign);
		aspell_string_enumeration_at_end=(PFaspell_string_enumeration_at_end)GetProcAddress(hInst,Naspell_string_enumeration_at_end);
		aspell_string_enumeration_next=(PFaspell_string_enumeration_next)GetProcAddress(hInst,Naspell_string_enumeration_next);
		//get_aspell_module_info_list=(PFget_aspell_module_info_list)GetProcAddress(hInst,Nget_aspell_module_info_list);
		//aspell_module_info_list_empty=(PFaspell_module_info_list_empty)GetProcAddress(hInst,Naspell_module_info_list_empty);
		//aspell_module_info_list_size=(PFaspell_module_info_list_size)GetProcAddress(hInst,Naspell_module_info_list_size);
		//aspell_module_info_list_elements=(PFaspell_module_info_list_elements)GetProcAddress(hInst,Naspell_module_info_list_elements);
		get_aspell_dict_info_list=(PFget_aspell_dict_info_list)GetProcAddress(hInst,Nget_aspell_dict_info_list);
		//aspell_dict_info_list_empty=(PFaspell_dict_info_list_empty)GetProcAddress(hInst,Naspell_dict_info_list_empty);
		//aspell_dict_info_list_size=(PFaspell_dict_info_list_size)GetProcAddress(hInst,Naspell_dict_info_list_size);
		aspell_dict_info_list_elements=(PFaspell_dict_info_list_elements)GetProcAddress(hInst,Naspell_dict_info_list_elements);
		//aspell_module_info_enumeration_at_end=(PFaspell_module_info_enumeration_at_end)GetProcAddress(hInst,Naspell_module_info_enumeration_at_end);
		//aspell_module_info_enumeration_next=(PFaspell_module_info_enumeration_next)GetProcAddress(hInst,Naspell_module_info_enumeration_next);
		//delete_aspell_module_info_enumeration=(PFdelete_aspell_module_info_enumeration)GetProcAddress(hInst,Ndelete_aspell_module_info_enumeration);
		//aspell_module_info_enumeration_clone=(PFaspell_module_info_enumeration_clone)GetProcAddress(hInst,Naspell_module_info_enumeration_clone);
		//aspell_module_info_enumeration_assign=(PFaspell_module_info_enumeration_assign)GetProcAddress(hInst,Naspell_module_info_enumeration_assign);
		//aspell_dict_info_enumeration_at_end=(PFaspell_dict_info_enumeration_at_end)GetProcAddress(hInst,Naspell_dict_info_enumeration_at_end);
		aspell_dict_info_enumeration_next=(PFaspell_dict_info_enumeration_next)GetProcAddress(hInst,Naspell_dict_info_enumeration_next);
		delete_aspell_dict_info_enumeration=(PFdelete_aspell_dict_info_enumeration)GetProcAddress(hInst,Ndelete_aspell_dict_info_enumeration);
		//aspell_dict_info_enumeration_clone=(PFaspell_dict_info_enumeration_clone)GetProcAddress(hInst,Naspell_dict_info_enumeration_clone);
		//aspell_dict_info_enumeration_assign=(PFaspell_dict_info_enumeration_assign)GetProcAddress(hInst,Naspell_dict_info_enumeration_assign);
		//new_aspell_string_list=(PFnew_aspell_string_list)GetProcAddress(hInst,Nnew_aspell_string_list);
		//aspell_string_list_empty=(PFaspell_string_list_empty)GetProcAddress(hInst,Naspell_string_list_empty);
		//aspell_string_list_size=(PFaspell_string_list_size)GetProcAddress(hInst,Naspell_string_list_size);
		//aspell_string_list_elements=(PFaspell_string_list_elements)GetProcAddress(hInst,Naspell_string_list_elements);
		//aspell_string_list_add=(PFaspell_string_list_add)GetProcAddress(hInst,Naspell_string_list_add);
		//aspell_string_list_remove=(PFaspell_string_list_remove)GetProcAddress(hInst,Naspell_string_list_remove);
		//aspell_string_list_clear=(PFaspell_string_list_clear)GetProcAddress(hInst,Naspell_string_list_clear);
		//aspell_string_list_to_mutable_container=(PFaspell_string_list_to_mutable_container)GetProcAddress(hInst,Naspell_string_list_to_mutable_container);
		//delete_aspell_string_list=(PFdelete_aspell_string_list)GetProcAddress(hInst,Ndelete_aspell_string_list);
		//aspell_string_list_clone=(PFaspell_string_list_clone)GetProcAddress(hInst,Naspell_string_list_clone);
		//aspell_string_list_assign=(PFaspell_string_list_assign)GetProcAddress(hInst,Naspell_string_list_assign);
		//new_aspell_string_map=(PFnew_aspell_string_map)GetProcAddress(hInst,Nnew_aspell_string_map);
		//aspell_string_map_add=(PFaspell_string_map_add)GetProcAddress(hInst,Naspell_string_map_add);
		//aspell_string_map_remove=(PFaspell_string_map_remove)GetProcAddress(hInst,Naspell_string_map_remove);
		//aspell_string_map_clear=(PFaspell_string_map_clear)GetProcAddress(hInst,Naspell_string_map_clear);
		//aspell_string_map_to_mutable_container=(PFaspell_string_map_to_mutable_container)GetProcAddress(hInst,Naspell_string_map_to_mutable_container);
		//delete_aspell_string_map=(PFdelete_aspell_string_map)GetProcAddress(hInst,Ndelete_aspell_string_map);
		//aspell_string_map_clone=(PFaspell_string_map_clone)GetProcAddress(hInst,Naspell_string_map_clone);
		//aspell_string_map_assign=(PFaspell_string_map_assign)GetProcAddress(hInst,Naspell_string_map_assign);
		//aspell_string_map_empty=(PFaspell_string_map_empty)GetProcAddress(hInst,Naspell_string_map_empty);
		//aspell_string_map_size=(PFaspell_string_map_size)GetProcAddress(hInst,Naspell_string_map_size);
		//aspell_string_map_elements=(PFaspell_string_map_elements)GetProcAddress(hInst,Naspell_string_map_elements);
		//aspell_string_map_insert=(PFaspell_string_map_insert)GetProcAddress(hInst,Naspell_string_map_insert);
		//aspell_string_map_replace=(PFaspell_string_map_replace)GetProcAddress(hInst,Naspell_string_map_replace);
		//aspell_string_map_lookup=(PFaspell_string_map_lookup)GetProcAddress(hInst,Naspell_string_map_lookup);
		//aspell_string_pair_enumeration_at_end=(PFaspell_string_pair_enumeration_at_end)GetProcAddress(hInst,Naspell_string_pair_enumeration_at_end);
		//aspell_string_pair_enumeration_next=(PFaspell_string_pair_enumeration_next)GetProcAddress(hInst,Naspell_string_pair_enumeration_next);
		//delete_aspell_string_pair_enumeration=(PFdelete_aspell_string_pair_enumeration)GetProcAddress(hInst,Ndelete_aspell_string_pair_enumeration);
		//aspell_string_pair_enumeration_clone=(PFaspell_string_pair_enumeration_clone)GetProcAddress(hInst,Naspell_string_pair_enumeration_clone);
		//aspell_string_pair_enumeration_assign=(PFaspell_string_pair_enumeration_assign)GetProcAddress(hInst,Naspell_string_pair_enumeration_assign);
	}
}

CAspellModule::~CAspellModule()
{
	if(hInst)
		FreeLibrary(hInst);
	hInst=NULL;
}
static void DECLSPEC_NORETURN WINAPI ExitProcess_Hooked(UINT code)
{
	throw new Excp(NULL);
}
BOOL CALLCONV CAspellModule::LoadModule(LPCWSTR path)
{
	if(theModule)
		return FALSE;
	theModule=new CAspellModule(path);
	if(!theModule)
		return FALSE;
	if(!theModule->hInst)
	{
		delete theModule;
		theModule=NULL;
		return FALSE;
	}
	PatchIat(GetModuleHandle(L"msvcrt.dll"),"KERNEL32.DLL","ExitProcess",(PVOID)ExitProcess_Hooked,(PVOID*)&rt.oldExitProcess);
	return TRUE;
}

void CALLCONV CAspellModule::UnLoadModule()
{
	if(theModule)
	{
		if(rt.oldExitProcess)
		{
			_ExitProcess_t trash;
			PatchIat(GetModuleHandle(L"msvcrt.dll"),"KERNEL32.DLL","ExitProcess",(PVOID)rt.oldExitProcess,(PVOID*)&trash);
			rt.oldExitProcess = NULL;
		}
		delete theModule;
		theModule=NULL;
	}
}
