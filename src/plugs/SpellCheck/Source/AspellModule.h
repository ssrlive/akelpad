#ifndef ASPELL_ASPELLMODULE__H
#define ASPELL_ASPELLMODULE__H
/**
 *		SpellChecker for AkelPad
 *		Panych Y.W. aka FeyFre (c) 2010-2014 (panych.y@gmail.com)
 *
 *		This piece of software is distributed under BSD license.
 *		AkelPad is property of its copyright holders.
 *		The New Aspell Copyright (C) 2001-2002 by Kevin Atkinson under the GNU LGPL
 *		license version 2.0 or 2.1.
 */

#include "AspellRaw.h"
#include "common.h"

class CAspellModule
{
private:
	// Closed constructor called thru LoadModule
	CAspellModule(LPCWSTR);
	// Closed destructor called tru UnLoadModule
	~CAspellModule();
	// Loaded aspell library instance
	HINSTANCE hInst;
	// Unique insatnce of class
public:
	static CAspellModule *theModule;
	//PFaspell_mutable_container_add aspell_mutable_container_add;
	//PFaspell_mutable_container_remove aspell_mutable_container_remove;
	//PFaspell_mutable_container_clear aspell_mutable_container_clear;
	//PFaspell_mutable_container_to_mutable_container aspell_mutable_container_to_mutable_container;
	//PFaspell_key_info_enumeration_at_end aspell_key_info_enumeration_at_end;
	//PFaspell_key_info_enumeration_next aspell_key_info_enumeration_next;
	//PFdelete_aspell_key_info_enumeration delete_aspell_key_info_enumeration;
	//PFaspell_key_info_enumeration_clone aspell_key_info_enumeration_clone;
	//PFaspell_key_info_enumeration_assign aspell_key_info_enumeration_assign;
	PFnew_aspell_config new_aspell_config;
	PFdelete_aspell_config delete_aspell_config;
	//PFaspell_config_clone aspell_config_clone;
	//PFaspell_config_assign aspell_config_assign;
	//PFaspell_config_error_number aspell_config_error_number;
	//PFaspell_config_error_message aspell_config_error_message;
	//PFaspell_config_error aspell_config_error;
	//PFaspell_config_set_extra aspell_config_set_extra;
	//PFaspell_config_keyinfo aspell_config_keyinfo;
	//PFaspell_config_possible_elements aspell_config_possible_elements;
	//PFaspell_config_get_default aspell_config_get_default;
	//PFaspell_config_elements aspell_config_elements;
	PFaspell_config_replace aspell_config_replace;
	//PFaspell_config_remove aspell_config_remove;
	//PFaspell_config_have aspell_config_have;
	//PFaspell_config_retrieve aspell_config_retrieve;
	//PFaspell_config_retrieve_list aspell_config_retrieve_list;
	//PFaspell_config_retrieve_bool aspell_config_retrieve_bool;
	//PFaspell_config_retrieve_int aspell_config_retrieve_int;
	//PFaspell_error_is_a aspell_error_is_a;
	//PFaspell_error_number aspell_error_number;
	PFaspell_error_message aspell_error_message;
	PFaspell_error aspell_error;
	PFdelete_aspell_can_have_error delete_aspell_can_have_error;
	PFnew_aspell_speller new_aspell_speller;
	PFto_aspell_speller to_aspell_speller;
	PFdelete_aspell_speller delete_aspell_speller;
	//PFaspell_speller_error_number aspell_speller_error_number;
	PFaspell_speller_error_message aspell_speller_error_message;
	//PFaspell_speller_error aspell_speller_error;
	PFaspell_speller_config aspell_speller_config;
	PFaspell_speller_check aspell_speller_check;
	//PFaspell_speller_add_to_personal aspell_speller_add_to_personal;
	//PFaspell_speller_add_to_session aspell_speller_add_to_session;
	//PFaspell_speller_personal_word_list aspell_speller_personal_word_list;
	//PFaspell_speller_session_word_list aspell_speller_session_word_list;
	//PFaspell_speller_main_word_list aspell_speller_main_word_list;
	//PFaspell_speller_save_all_word_lists aspell_speller_save_all_word_lists;
	//PFaspell_speller_clear_session aspell_speller_clear_session;
	PFaspell_speller_suggest aspell_speller_suggest;
	//PFaspell_speller_store_replacement aspell_speller_store_replacement;
	//PFdelete_aspell_filter delete_aspell_filter;
	//PFaspell_filter_error_number aspell_filter_error_number;
	//PFaspell_filter_error_message aspell_filter_error_message;
	//PFaspell_filter_error aspell_filter_error;
	//PFto_aspell_filter to_aspell_filter;
	//PFdelete_aspell_document_checker delete_aspell_document_checker;
	//PFaspell_document_checker_error_number aspell_document_checker_error_number;
	//PFaspell_document_checker_error_message aspell_document_checker_error_message;
	//PFaspell_document_checker_error aspell_document_checker_error;
	//PFnew_aspell_document_checker new_aspell_document_checker;
	//PFto_aspell_document_checker to_aspell_document_checker;
	//PFaspell_document_checker_reset aspell_document_checker_reset;
	//PFaspell_document_checker_process aspell_document_checker_process;
	//PFaspell_document_checker_next_misspelling aspell_document_checker_next_misspelling;
	//PFaspell_document_checker_filter aspell_document_checker_filter;
	//PFaspell_word_list_empty aspell_word_list_empty;
	//PFaspell_word_list_size aspell_word_list_size;
	PFaspell_word_list_elements aspell_word_list_elements;
	PFdelete_aspell_string_enumeration delete_aspell_string_enumeration;
	//PFaspell_string_enumeration_clone aspell_string_enumeration_clone;
	//PFaspell_string_enumeration_assign aspell_string_enumeration_assign;
	PFaspell_string_enumeration_at_end aspell_string_enumeration_at_end;
	PFaspell_string_enumeration_next aspell_string_enumeration_next;
	//PFget_aspell_module_info_list get_aspell_module_info_list;
	//PFaspell_module_info_list_empty aspell_module_info_list_empty;
	//PFaspell_module_info_list_size aspell_module_info_list_size;
	//PFaspell_module_info_list_elements aspell_module_info_list_elements;
	PFget_aspell_dict_info_list get_aspell_dict_info_list;
	//PFaspell_dict_info_list_empty aspell_dict_info_list_empty;
	//PFaspell_dict_info_list_size aspell_dict_info_list_size;
	PFaspell_dict_info_list_elements aspell_dict_info_list_elements;
	//PFaspell_module_info_enumeration_at_end aspell_module_info_enumeration_at_end;
	//PFaspell_module_info_enumeration_next aspell_module_info_enumeration_next;
	//PFdelete_aspell_module_info_enumeration delete_aspell_module_info_enumeration;
	//PFaspell_module_info_enumeration_clone aspell_module_info_enumeration_clone;
	//PFaspell_module_info_enumeration_assign aspell_module_info_enumeration_assign;
	//PFaspell_dict_info_enumeration_at_end aspell_dict_info_enumeration_at_end;
	PFaspell_dict_info_enumeration_next aspell_dict_info_enumeration_next;
	PFdelete_aspell_dict_info_enumeration delete_aspell_dict_info_enumeration;
	//PFaspell_dict_info_enumeration_clone aspell_dict_info_enumeration_clone;
	//PFaspell_dict_info_enumeration_assign aspell_dict_info_enumeration_assign;
	//PFnew_aspell_string_list new_aspell_string_list;
	//PFaspell_string_list_empty aspell_string_list_empty;
	//PFaspell_string_list_size aspell_string_list_size;
	//PFaspell_string_list_elements aspell_string_list_elements;
	//PFaspell_string_list_add aspell_string_list_add;
	//PFaspell_string_list_remove aspell_string_list_remove;
	//PFaspell_string_list_clear aspell_string_list_clear;
	//PFaspell_string_list_to_mutable_container aspell_string_list_to_mutable_container;
	//PFdelete_aspell_string_list delete_aspell_string_list;
	//PFaspell_string_list_clone aspell_string_list_clone;
	//PFaspell_string_list_assign aspell_string_list_assign;
	//PFnew_aspell_string_map new_aspell_string_map;
	//PFaspell_string_map_add aspell_string_map_add;
	//PFaspell_string_map_remove aspell_string_map_remove;
	//PFaspell_string_map_clear aspell_string_map_clear;
	//PFaspell_string_map_to_mutable_container aspell_string_map_to_mutable_container;
	//PFdelete_aspell_string_map delete_aspell_string_map;
	//PFaspell_string_map_clone aspell_string_map_clone;
	//PFaspell_string_map_assign aspell_string_map_assign;
	//PFaspell_string_map_empty aspell_string_map_empty;
	//PFaspell_string_map_size aspell_string_map_size;
	//PFaspell_string_map_elements aspell_string_map_elements;
	//PFaspell_string_map_insert aspell_string_map_insert;
	//PFaspell_string_map_replace aspell_string_map_replace;
	//PFaspell_string_map_lookup aspell_string_map_lookup;
	//PFaspell_string_pair_enumeration_at_end aspell_string_pair_enumeration_at_end;
	//PFaspell_string_pair_enumeration_next aspell_string_pair_enumeration_next;
	//PFdelete_aspell_string_pair_enumeration delete_aspell_string_pair_enumeration;
	//PFaspell_string_pair_enumeration_clone aspell_string_pair_enumeration_clone;
	//PFaspell_string_pair_enumeration_assign aspell_string_pair_enumeration_assign;
public:
	// Aspell loader
	static BOOL CALLCONV LoadModule(LPCWSTR);
	// Aspell unloader
	static void CALLCONV UnLoadModule();
	// All aspell atom wrappers are friends
	static CAspellModule* CALLCONV GetModule(){return CAspellModule::theModule;}
};
#endif
