#ifndef __ASPELLRAWASPELL_H__
#define __ASPELLRAWASPELL_H__
/**
 *		SpellChecker for AkelPad
 *		Panych Y.W. aka FeyFre (c) 2010-2014 (panych.y@gmail.com)
 *
 *		This piece of software is distributed under BSD license.
 *		AkelPad is property of its copyright holders.
 *		The New Aspell Copyright (C) 2001-2002 by Kevin Atkinson under the GNU LGPL
 *		license version 2.0 or 2.1.
 */
#include "aspell.h"
//union AspellTypeId
//{
//	unsigned int num;
//	char str[4];
//};
//typedef union AspellTypeId AspellTypeId;
//typedef struct AspellMutableContainer AspellMutableContainer;
//typedef enum AspellKeyInfoType AspellKeyInfoType;
//struct AspellKeyInfo
//{
//	const char * name;
//	enum AspellKeyInfoType type;
//	const char * def;
//	const char * desc;
//	char otherdata[16];
//};
//typedef struct AspellKeyInfo AspellKeyInfo;
//typedef struct AspellKeyInfoEnumeration AspellKeyInfoEnumeration;
//typedef struct AspellConfig AspellConfig;
//typedef struct AspellError AspellError;
//struct AspellError
//{
//	const char * mesg;
//	const struct AspellErrorInfo * err;
//};
//typedef struct AspellErrorInfo AspellErrorInfo;
//typedef struct AspellCanHaveError AspellCanHaveError;
//typedef struct AspellSpeller AspellSpeller;
//typedef struct AspellFilter AspellFilter;
//typedef struct AspellToken AspellToken;
//typedef struct AspellDocumentChecker AspellDocumentChecker;
//typedef struct AspellWordList AspellWordList;
//typedef struct AspellStringEnumeration AspellStringEnumeration;
//enum AspellKeyInfoType {AspellKeyInfoString, AspellKeyInfoInt, AspellKeyInfoBool, AspellKeyInfoList};
//struct AspellErrorInfo
//{
//	const struct AspellErrorInfo * isa;
//	const char * mesg;
//	unsigned int num_parms;
//	const char * parms[3];
//};
//struct AspellToken
//{
//	unsigned int offset;
//	unsigned int len;
//};
//typedef struct AspellStringList AspellStringList;
//typedef struct AspellStringMap AspellStringMap;
//struct AspellModuleInfo
//{
//	const char * name;
//	double order_num;
//	const char * lib_dir;
//	struct AspellStringList * dict_dirs;
//	struct AspellStringList * dict_exts;
//};
//typedef struct AspellModuleInfo AspellModuleInfo;
//struct AspellDictInfo
//{
//	const char * name;
//	const char * code;
//	const char * jargon;
//	int size;
//	const char * size_str;
//	struct AspellModuleInfo * module;
//};
//typedef struct AspellDictInfo AspellDictInfo;
//typedef struct AspellModuleInfoList AspellModuleInfoList;
//typedef struct AspellDictInfoList AspellDictInfoList;
//typedef struct AspellModuleInfoEnumeration AspellModuleInfoEnumeration;
//typedef struct AspellDictInfoEnumeration AspellDictInfoEnumeration;
//
//struct AspellStringPair
//{
//	const char * first;
//	const char * second;
//};
//typedef struct AspellStringPair AspellStringPair;
//typedef struct AspellStringPairEnumeration AspellStringPairEnumeration;
//
//#ifdef __cplusplus
//extern "C" {
//#endif
///*extern const struct AspellErrorInfo * const aerror_other;
//extern const struct AspellErrorInfo * const aerror_operation_not_supported;
//extern const struct AspellErrorInfo * const aerror_cant_copy;
//extern const struct AspellErrorInfo * const aerror_file;
//extern const struct AspellErrorInfo * const aerror_cant_open_file;
//extern const struct AspellErrorInfo * const aerror_cant_read_file;
//extern const struct AspellErrorInfo * const aerror_cant_write_file;
//extern const struct AspellErrorInfo * const aerror_invalid_name;
//extern const struct AspellErrorInfo * const aerror_bad_file_format;
//extern const struct AspellErrorInfo * const aerror_dir;
//extern const struct AspellErrorInfo * const aerror_cant_read_dir;
//extern const struct AspellErrorInfo * const aerror_config;
//extern const struct AspellErrorInfo * const aerror_unknown_key;
//extern const struct AspellErrorInfo * const aerror_cant_change_value;
//extern const struct AspellErrorInfo * const aerror_bad_key;
//extern const struct AspellErrorInfo * const aerror_bad_value;
//extern const struct AspellErrorInfo * const aerror_duplicate;
//extern const struct AspellErrorInfo * const aerror_language_related;
//extern const struct AspellErrorInfo * const aerror_unknown_language;
//extern const struct AspellErrorInfo * const aerror_unknown_soundslike;
//extern const struct AspellErrorInfo * const aerror_language_not_supported;
//extern const struct AspellErrorInfo * const aerror_no_wordlist_for_lang;
//extern const struct AspellErrorInfo * const aerror_mismatched_language;
//extern const struct AspellErrorInfo * const aerror_encoding;
//extern const struct AspellErrorInfo * const aerror_unknown_encoding;
//extern const struct AspellErrorInfo * const aerror_encoding_not_supported;
//extern const struct AspellErrorInfo * const aerror_conversion_not_supported;
//extern const struct AspellErrorInfo * const aerror_pipe;
//extern const struct AspellErrorInfo * const aerror_cant_create_pipe;
//extern const struct AspellErrorInfo * const aerror_process_died;
//extern const struct AspellErrorInfo * const aerror_bad_input;
//extern const struct AspellErrorInfo * const aerror_invalid_word;
//extern const struct AspellErrorInfo * const aerror_word_list_flags;
//extern const struct AspellErrorInfo * const aerror_invalid_flag;
//extern const struct AspellErrorInfo * const aerror_conflicting_flags;*/
//#ifdef __cplusplus
//}
//#endif

typedef int (*PFaspell_mutable_container_add)(struct AspellMutableContainer * ths, const char * to_add);
#define Naspell_mutable_container_add "aspell_mutable_container_add"
typedef int (*PFaspell_mutable_container_remove)(struct AspellMutableContainer * ths, const char * to_rem);
#define Naspell_mutable_container_remove "aspell_mutable_container_remove"
typedef void (*PFaspell_mutable_container_clear)(struct AspellMutableContainer * ths);
#define Naspell_mutable_container_clear "aspell_mutable_container_clear"
typedef struct AspellMutableContainer * (*PFaspell_mutable_container_to_mutable_container)(struct AspellMutableContainer * ths);
#define Naspell_mutable_container_to_mutable_container "aspell_mutable_container_to_mutable_container"
typedef int (*PFaspell_key_info_enumeration_at_end)(const struct AspellKeyInfoEnumeration * ths);
#define Naspell_key_info_enumeration_at_end "aspell_key_info_enumeration_at_end"
typedef const struct AspellKeyInfo * (*PFaspell_key_info_enumeration_next)(struct AspellKeyInfoEnumeration * ths);
#define Naspell_key_info_enumeration_next "aspell_key_info_enumeration_next"
typedef void (*PFdelete_aspell_key_info_enumeration)(struct AspellKeyInfoEnumeration * ths);
#define Ndelete_aspell_key_info_enumeration "delete_aspell_key_info_enumeration"
typedef struct AspellKeyInfoEnumeration * (*PFaspell_key_info_enumeration_clone)(const struct AspellKeyInfoEnumeration * ths);
#define Naspell_key_info_enumeration_clone "aspell_key_info_enumeration_clone"
typedef void (*PFaspell_key_info_enumeration_assign)(struct AspellKeyInfoEnumeration * ths, const struct AspellKeyInfoEnumeration * other);
#define Naspell_key_info_enumeration_assign "aspell_key_info_enumeration_assign"
typedef struct AspellConfig * (*PFnew_aspell_config)();
#define Nnew_aspell_config "new_aspell_config"
typedef void (*PFdelete_aspell_config)(struct AspellConfig * ths);
#define Ndelete_aspell_config "delete_aspell_config"
typedef struct AspellConfig * (*PFaspell_config_clone)(const struct AspellConfig * ths);
#define Naspell_config_clone "aspell_config_clone"
typedef void (*PFaspell_config_assign)(struct AspellConfig * ths, const struct AspellConfig * other);
#define Naspell_config_assign "aspell_config_assign"
typedef unsigned int (*PFaspell_config_error_number)(const struct AspellConfig * ths);
#define Naspell_config_error_number "aspell_config_error_number"
typedef const char * (*PFaspell_config_error_message)(const struct AspellConfig * ths);
#define Naspell_config_error_message "aspell_config_error_message"
typedef const struct AspellError * (*PFaspell_config_error)(const struct AspellConfig * ths);
#define Naspell_config_error "aspell_config_error"
typedef void (*PFaspell_config_set_extra)(struct AspellConfig * ths, const struct AspellKeyInfo * begin, const struct AspellKeyInfo * end);
#define Naspell_config_set_extra "aspell_config_set_extra"
typedef const struct AspellKeyInfo * (*PFaspell_config_keyinfo)(struct AspellConfig * ths, const char * key);
#define Naspell_config_keyinfo "aspell_config_keyinfo"
typedef struct AspellKeyInfoEnumeration * (*PFaspell_config_possible_elements)(struct AspellConfig * ths, int include_extra);
#define Naspell_config_possible_elements "aspell_config_possible_elements"
typedef const char * (*PFaspell_config_get_default)(struct AspellConfig * ths, const char * key);
#define Naspell_config_get_default "aspell_config_get_default"
typedef struct AspellStringPairEnumeration * (*PFaspell_config_elements)(struct AspellConfig * ths);
#define Naspell_config_elements "aspell_config_elements"
typedef int (*PFaspell_config_replace)(struct AspellConfig * ths, const char * key, const char * value);
#define Naspell_config_replace "aspell_config_replace"
typedef int (*PFaspell_config_remove)(struct AspellConfig * ths, const char * key);
#define Naspell_config_remove "aspell_config_remove"
typedef int (*PFaspell_config_have)(const struct AspellConfig * ths, const char * key);
#define Naspell_config_have "aspell_config_have"
typedef const char * (*PFaspell_config_retrieve)(struct AspellConfig * ths, const char * key);
#define Naspell_config_retrieve "aspell_config_retrieve"
typedef int (*PFaspell_config_retrieve_list)(struct AspellConfig * ths, const char * key, struct AspellMutableContainer * lst);
#define Naspell_config_retrieve_list "aspell_config_retrieve_list"
typedef int (*PFaspell_config_retrieve_bool)(struct AspellConfig * ths, const char * key);
#define Naspell_config_retrieve_bool "aspell_config_retrieve_bool"
typedef int (*PFaspell_config_retrieve_int)(struct AspellConfig * ths, const char * key);
#define Naspell_config_retrieve_int "aspell_config_retrieve_int"
typedef int (*PFaspell_error_is_a)(const struct AspellError * ths, const struct AspellErrorInfo * e);
#define Naspell_error_is_a "aspell_error_is_a"
typedef unsigned int (*PFaspell_error_number)(const struct AspellCanHaveError * ths);
#define Naspell_error_number "aspell_error_number"
typedef const char * (*PFaspell_error_message)(const struct AspellCanHaveError * ths);
#define Naspell_error_message "aspell_error_message"
typedef const struct AspellError * (*PFaspell_error)(const struct AspellCanHaveError * ths);
#define Naspell_error "aspell_error"
typedef void (*PFdelete_aspell_can_have_error)(struct AspellCanHaveError * ths);
#define Ndelete_aspell_can_have_error "delete_aspell_can_have_error"
typedef struct AspellCanHaveError * (*PFnew_aspell_speller)(struct AspellConfig * config);
#define Nnew_aspell_speller "new_aspell_speller"
typedef struct AspellSpeller * (*PFto_aspell_speller)(struct AspellCanHaveError * obj);
#define Nto_aspell_speller "to_aspell_speller"
typedef void (*PFdelete_aspell_speller)(struct AspellSpeller * ths);
#define Ndelete_aspell_speller "delete_aspell_speller"
typedef unsigned int (*PFaspell_speller_error_number)(const struct AspellSpeller * ths);
#define Naspell_speller_error_number "aspell_speller_error_number"
typedef const char * (*PFaspell_speller_error_message)(const struct AspellSpeller * ths);
#define Naspell_speller_error_message "aspell_speller_error_message"
typedef const struct AspellError * (*PFaspell_speller_error)(const struct AspellSpeller * ths);
#define Naspell_speller_error "aspell_speller_error"
typedef struct AspellConfig * (*PFaspell_speller_config)(struct AspellSpeller * ths);
#define Naspell_speller_config "aspell_speller_config"
typedef int (*PFaspell_speller_check)(struct AspellSpeller * ths, const char * word, int word_size);
#define Naspell_speller_check "aspell_speller_check"
typedef int (*PFaspell_speller_add_to_personal)(struct AspellSpeller * ths, const char * word, int word_size);
#define Naspell_speller_add_to_personal "aspell_speller_add_to_personal"
typedef int (*PFaspell_speller_add_to_session)(struct AspellSpeller * ths, const char * word, int word_size);
#define Naspell_speller_add_to_session "aspell_speller_add_to_session"
typedef const struct AspellWordList * (*PFaspell_speller_personal_word_list)(struct AspellSpeller * ths);
#define Naspell_speller_personal_word_list "aspell_speller_personal_word_list"
typedef const struct AspellWordList * (*PFaspell_speller_session_word_list)(struct AspellSpeller * ths);
#define Naspell_speller_session_word_list "aspell_speller_session_word_list"
typedef const struct AspellWordList * (*PFaspell_speller_main_word_list)(struct AspellSpeller * ths);
#define Naspell_speller_main_word_list "aspell_speller_main_word_list"
typedef int (*PFaspell_speller_save_all_word_lists)(struct AspellSpeller * ths);
#define Naspell_speller_save_all_word_lists "aspell_speller_save_all_word_lists"
typedef int (*PFaspell_speller_clear_session)(struct AspellSpeller * ths);
#define Naspell_speller_clear_session "aspell_speller_clear_session"
typedef const struct AspellWordList * (*PFaspell_speller_suggest)(struct AspellSpeller * ths, const char * word, int word_size);
#define Naspell_speller_suggest "aspell_speller_suggest"
typedef int (*PFaspell_speller_store_replacement)(struct AspellSpeller * ths, const char * mis, int mis_size, const char * cor, int cor_size);
#define Naspell_speller_store_replacement "aspell_speller_store_replacement"
typedef void (*PFdelete_aspell_filter)(struct AspellFilter * ths);
#define Ndelete_aspell_filter "delete_aspell_filter"
typedef unsigned int (*PFaspell_filter_error_number)(const struct AspellFilter * ths);
#define Naspell_filter_error_number "aspell_filter_error_number"
typedef const char * (*PFaspell_filter_error_message)(const struct AspellFilter * ths);
#define Naspell_filter_error_message "aspell_filter_error_message"
typedef const struct AspellError * (*PFaspell_filter_error)(const struct AspellFilter * ths);
#define Naspell_filter_error "aspell_filter_error"
typedef struct AspellFilter * (*PFto_aspell_filter)(struct AspellCanHaveError * obj);
#define Nto_aspell_filter "to_aspell_filter"
typedef void (*PFdelete_aspell_document_checker)(struct AspellDocumentChecker * ths);
#define Ndelete_aspell_document_checker "delete_aspell_document_checker"
typedef unsigned int (*PFaspell_document_checker_error_number)(const struct AspellDocumentChecker * ths);
#define Naspell_document_checker_error_number "aspell_document_checker_error_number"
typedef const char * (*PFaspell_document_checker_error_message)(const struct AspellDocumentChecker * ths);
#define Naspell_document_checker_error_message "aspell_document_checker_error_message"
typedef const struct AspellError * (*PFaspell_document_checker_error)(const struct AspellDocumentChecker * ths);
#define Naspell_document_checker_error "aspell_document_checker_error"
typedef struct AspellCanHaveError * (*PFnew_aspell_document_checker)(struct AspellSpeller * speller);
#define Nnew_aspell_document_checker "new_aspell_document_checker"
typedef struct AspellDocumentChecker * (*PFto_aspell_document_checker)(struct AspellCanHaveError * obj);
#define Nto_aspell_document_checker "to_aspell_document_checker"
typedef void (*PFaspell_document_checker_reset)(struct AspellDocumentChecker * ths);
#define Naspell_document_checker_reset "aspell_document_checker_reset"
typedef void (*PFaspell_document_checker_process)(struct AspellDocumentChecker * ths, const char * str, int size);
#define Naspell_document_checker_process "aspell_document_checker_process"
typedef struct AspellToken (*PFaspell_document_checker_next_misspelling)(struct AspellDocumentChecker * ths);
#define Naspell_document_checker_next_misspelling "aspell_document_checker_next_misspelling"
typedef struct AspellFilter * (*PFaspell_document_checker_filter)(struct AspellDocumentChecker * ths);
#define Naspell_document_checker_filter "aspell_document_checker_filter"
typedef int (*PFaspell_word_list_empty)(const struct AspellWordList * ths);
#define Naspell_word_list_empty "aspell_word_list_empty"
typedef unsigned int (*PFaspell_word_list_size)(const struct AspellWordList * ths);
#define Naspell_word_list_size "aspell_word_list_size"
typedef struct AspellStringEnumeration * (*PFaspell_word_list_elements)(const struct AspellWordList * ths);
#define Naspell_word_list_elements "aspell_word_list_elements"
typedef void (*PFdelete_aspell_string_enumeration)(struct AspellStringEnumeration * ths);
#define Ndelete_aspell_string_enumeration "delete_aspell_string_enumeration"
typedef struct AspellStringEnumeration * (*PFaspell_string_enumeration_clone)(const struct AspellStringEnumeration * ths);
#define Naspell_string_enumeration_clone "aspell_string_enumeration_clone"
typedef void (*PFaspell_string_enumeration_assign)(struct AspellStringEnumeration * ths, const struct AspellStringEnumeration * other);
#define Naspell_string_enumeration_assign "aspell_string_enumeration_assign"
typedef int (*PFaspell_string_enumeration_at_end)(const struct AspellStringEnumeration * ths);
#define Naspell_string_enumeration_at_end "aspell_string_enumeration_at_end"
typedef const char * (*PFaspell_string_enumeration_next)(struct AspellStringEnumeration * ths);
#define Naspell_string_enumeration_next "aspell_string_enumeration_next"
typedef struct AspellModuleInfoList * (*PFget_aspell_module_info_list)(struct AspellConfig * config);
#define Nget_aspell_module_info_list "get_aspell_module_info_list"
typedef int (*PFaspell_module_info_list_empty)(const struct AspellModuleInfoList * ths);
#define Naspell_module_info_list_empty "aspell_module_info_list_empty"
typedef unsigned int (*PFaspell_module_info_list_size)(const struct AspellModuleInfoList * ths);
#define Naspell_module_info_list_size "aspell_module_info_list_size"
typedef struct AspellModuleInfoEnumeration * (*PFaspell_module_info_list_elements)(const struct AspellModuleInfoList * ths);
#define Naspell_module_info_list_elements "aspell_module_info_list_elements"
typedef struct AspellDictInfoList * (*PFget_aspell_dict_info_list)(struct AspellConfig * config);
#define Nget_aspell_dict_info_list "get_aspell_dict_info_list"
typedef int (*PFaspell_dict_info_list_empty)(const struct AspellDictInfoList * ths);
#define Naspell_dict_info_list_empty "aspell_dict_info_list_empty"
typedef unsigned int (*PFaspell_dict_info_list_size)(const struct AspellDictInfoList * ths);
#define Naspell_dict_info_list_size "aspell_dict_info_list_size"
typedef struct AspellDictInfoEnumeration * (*PFaspell_dict_info_list_elements)(const struct AspellDictInfoList * ths);
#define Naspell_dict_info_list_elements "aspell_dict_info_list_elements"
typedef int (*PFaspell_module_info_enumeration_at_end)(const struct AspellModuleInfoEnumeration * ths);
#define Naspell_module_info_enumeration_at_end "aspell_module_info_enumeration_at_end"
typedef const struct AspellModuleInfo * (*PFaspell_module_info_enumeration_next)(struct AspellModuleInfoEnumeration * ths);
#define Naspell_module_info_enumeration_next "aspell_module_info_enumeration_next"
typedef void (*PFdelete_aspell_module_info_enumeration)(struct AspellModuleInfoEnumeration * ths);
#define Ndelete_aspell_module_info_enumeration "delete_aspell_module_info_enumeration"
typedef struct AspellModuleInfoEnumeration * (*PFaspell_module_info_enumeration_clone)(const struct AspellModuleInfoEnumeration * ths);
#define Naspell_module_info_enumeration_clone "aspell_module_info_enumeration_clone"
typedef void (*PFaspell_module_info_enumeration_assign)(struct AspellModuleInfoEnumeration * ths, const struct AspellModuleInfoEnumeration * other);
#define Naspell_module_info_enumeration_assign "aspell_module_info_enumeration_assign"
typedef int (*PFaspell_dict_info_enumeration_at_end)(const struct AspellDictInfoEnumeration * ths);
#define Naspell_dict_info_enumeration_at_end "aspell_dict_info_enumeration_at_end"
typedef const struct AspellDictInfo * (*PFaspell_dict_info_enumeration_next)(struct AspellDictInfoEnumeration * ths);
#define Naspell_dict_info_enumeration_next "aspell_dict_info_enumeration_next"
typedef void (*PFdelete_aspell_dict_info_enumeration)(struct AspellDictInfoEnumeration * ths);
#define Ndelete_aspell_dict_info_enumeration "delete_aspell_dict_info_enumeration"
typedef struct AspellDictInfoEnumeration * (*PFaspell_dict_info_enumeration_clone)(const struct AspellDictInfoEnumeration * ths);
#define Naspell_dict_info_enumeration_clone "aspell_dict_info_enumeration_clone"
typedef void (*PFaspell_dict_info_enumeration_assign)(struct AspellDictInfoEnumeration * ths, const struct AspellDictInfoEnumeration * other);
#define Naspell_dict_info_enumeration_assign "aspell_dict_info_enumeration_assign"
typedef struct AspellStringList * (*PFnew_aspell_string_list)();
#define Nnew_aspell_string_list "new_aspell_string_list"
typedef int (*PFaspell_string_list_empty)(const struct AspellStringList * ths);
#define Naspell_string_list_empty "aspell_string_list_empty"
typedef unsigned int (*PFaspell_string_list_size)(const struct AspellStringList * ths);
#define Naspell_string_list_size "aspell_string_list_size"
typedef struct AspellStringEnumeration * (*PFaspell_string_list_elements)(const struct AspellStringList * ths);
#define Naspell_string_list_elements "aspell_string_list_elements"
typedef int (*PFaspell_string_list_add)(struct AspellStringList * ths, const char * to_add);
#define Naspell_string_list_add "aspell_string_list_add"
typedef int (*PFaspell_string_list_remove)(struct AspellStringList * ths, const char * to_rem);
#define Naspell_string_list_remove "aspell_string_list_remove"
typedef void (*PFaspell_string_list_clear)(struct AspellStringList * ths);
#define Naspell_string_list_clear "aspell_string_list_clear"
typedef struct AspellMutableContainer * (*PFaspell_string_list_to_mutable_container)(struct AspellStringList * ths);
#define Naspell_string_list_to_mutable_container "aspell_string_list_to_mutable_container"
typedef void (*PFdelete_aspell_string_list)(struct AspellStringList * ths);
#define Ndelete_aspell_string_list "delete_aspell_string_list"
typedef struct AspellStringList * (*PFaspell_string_list_clone)(const struct AspellStringList * ths);
#define Naspell_string_list_clone "aspell_string_list_clone"
typedef void (*PFaspell_string_list_assign)(struct AspellStringList * ths, const struct AspellStringList * other);
#define Naspell_string_list_assign "aspell_string_list_assign"
typedef struct AspellStringMap * (*PFnew_aspell_string_map)();
#define Nnew_aspell_string_map "new_aspell_string_map"
typedef int (*PFaspell_string_map_add)(struct AspellStringMap * ths, const char * to_add);
#define Naspell_string_map_add "aspell_string_map_add"
typedef int (*PFaspell_string_map_remove)(struct AspellStringMap * ths, const char * to_rem);
#define Naspell_string_map_remove "aspell_string_map_remove"
typedef void (*PFaspell_string_map_clear)(struct AspellStringMap * ths);
#define Naspell_string_map_clear "aspell_string_map_clear"
typedef struct AspellMutableContainer * (*PFaspell_string_map_to_mutable_container)(struct AspellStringMap * ths);
#define Naspell_string_map_to_mutable_container "aspell_string_map_to_mutable_container"
typedef void (*PFdelete_aspell_string_map)(struct AspellStringMap * ths);
#define Ndelete_aspell_string_map "delete_aspell_string_map"
typedef struct AspellStringMap * (*PFaspell_string_map_clone)(const struct AspellStringMap * ths);
#define Naspell_string_map_clone "aspell_string_map_clone"
typedef void (*PFaspell_string_map_assign)(struct AspellStringMap * ths, const struct AspellStringMap * other);
#define Naspell_string_map_assign "aspell_string_map_assign"
typedef int (*PFaspell_string_map_empty)(const struct AspellStringMap * ths);
#define Naspell_string_map_empty "aspell_string_map_empty"
typedef unsigned int (*PFaspell_string_map_size)(const struct AspellStringMap * ths);
#define Naspell_string_map_size "aspell_string_map_size"
typedef struct AspellStringPairEnumeration * (*PFaspell_string_map_elements)(const struct AspellStringMap * ths);
#define Naspell_string_map_elements "aspell_string_map_elements"
typedef int (*PFaspell_string_map_insert)(struct AspellStringMap * ths, const char * key, const char * value);
#define Naspell_string_map_insert "aspell_string_map_insert"
typedef int (*PFaspell_string_map_replace)(struct AspellStringMap * ths, const char * key, const char * value);
#define Naspell_string_map_replace "aspell_string_map_replace"
typedef const char * (*PFaspell_string_map_lookup)(const struct AspellStringMap * ths, const char * key);
#define Naspell_string_map_lookup "aspell_string_map_lookup"
typedef int (*PFaspell_string_pair_enumeration_at_end)(const struct AspellStringPairEnumeration * ths);
#define Naspell_string_pair_enumeration_at_end "aspell_string_pair_enumeration_at_end"
typedef struct AspellStringPair (*PFaspell_string_pair_enumeration_next)(struct AspellStringPairEnumeration * ths);
#define Naspell_string_pair_enumeration_next "aspell_string_pair_enumeration_next"
typedef void (*PFdelete_aspell_string_pair_enumeration)(struct AspellStringPairEnumeration * ths);
#define Ndelete_aspell_string_pair_enumeration "delete_aspell_string_pair_enumeration"
typedef struct AspellStringPairEnumeration * (*PFaspell_string_pair_enumeration_clone)(const struct AspellStringPairEnumeration * ths);
#define Naspell_string_pair_enumeration_clone "aspell_string_pair_enumeration_clone"
typedef void (*PFaspell_string_pair_enumeration_assign)(struct AspellStringPairEnumeration * ths, const struct AspellStringPairEnumeration * other);
#define Naspell_string_pair_enumeration_assign "aspell_string_pair_enumeration_assign"

#endif //__ASPELLRAWASPELL_H__
