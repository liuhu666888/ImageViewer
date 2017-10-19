#if !defined (FT_PREFIX_H_INCLUDED)
#define FT_PREFIX_H_INCLUDED

#define FT_MSB_USE_C_IMPL 1

/*
nm -C libtp_freetype.a | grep -E '^[a-z0-9]{16} [TDBR]' | sed 's|^.* . \(.*\)$|#define \1 tp_\1|' | LANG=C sort -u
*/

#define FTC_CMapCache_Lookup tp_FTC_CMapCache_Lookup
#define FTC_CMapCache_New tp_FTC_CMapCache_New
#define FTC_ImageCache_Lookup tp_FTC_ImageCache_Lookup
#define FTC_ImageCache_LookupScaler tp_FTC_ImageCache_LookupScaler
#define FTC_ImageCache_New tp_FTC_ImageCache_New
#define FTC_Manager_Done tp_FTC_Manager_Done
#define FTC_Manager_LookupFace tp_FTC_Manager_LookupFace
#define FTC_Manager_LookupSize tp_FTC_Manager_LookupSize
#define FTC_Manager_New tp_FTC_Manager_New
#define FTC_Manager_RemoveFaceID tp_FTC_Manager_RemoveFaceID
#define FTC_Manager_Reset tp_FTC_Manager_Reset
#define FTC_Node_Unref tp_FTC_Node_Unref
#define FTC_SBitCache_Lookup tp_FTC_SBitCache_Lookup
#define FTC_SBitCache_LookupScaler tp_FTC_SBitCache_LookupScaler
#define FTC_SBitCache_New tp_FTC_SBitCache_New
#define FT_Activate_Size tp_FT_Activate_Size
#define FT_Add_Default_Modules tp_FT_Add_Default_Modules
#define FT_Add_Module tp_FT_Add_Module
#define FT_Angle_Diff tp_FT_Angle_Diff
#define FT_Atan2 tp_FT_Atan2
#define FT_Attach_File tp_FT_Attach_File
#define FT_Attach_Stream tp_FT_Attach_Stream
#define FT_Bitmap_Convert tp_FT_Bitmap_Convert
#define FT_Bitmap_Copy tp_FT_Bitmap_Copy
#define FT_Bitmap_Done tp_FT_Bitmap_Done
#define FT_Bitmap_Embolden tp_FT_Bitmap_Embolden
#define FT_Bitmap_Init tp_FT_Bitmap_Init
#define FT_Bitmap_New tp_FT_Bitmap_New
#define FT_CMap_Done tp_FT_CMap_Done
#define FT_CMap_New tp_FT_CMap_New
#define FT_CeilFix tp_FT_CeilFix
#define FT_Cos tp_FT_Cos
#define FT_DivFix tp_FT_DivFix
#define FT_Done_Face tp_FT_Done_Face
#define FT_Done_FreeType tp_FT_Done_FreeType
#define FT_Done_Glyph tp_FT_Done_Glyph
#define FT_Done_GlyphSlot tp_FT_Done_GlyphSlot
#define FT_Done_Library tp_FT_Done_Library
#define FT_Done_Memory tp_FT_Done_Memory
#define FT_Done_Size tp_FT_Done_Size
#define FT_Face_GetCharVariantIndex tp_FT_Face_GetCharVariantIndex
#define FT_Face_GetCharVariantIsDefault tp_FT_Face_GetCharVariantIsDefault
#define FT_Face_GetCharsOfVariant tp_FT_Face_GetCharsOfVariant
#define FT_Face_GetVariantSelectors tp_FT_Face_GetVariantSelectors
#define FT_Face_GetVariantsOfChar tp_FT_Face_GetVariantsOfChar
#define FT_Face_Properties tp_FT_Face_Properties
#define FT_FloorFix tp_FT_FloorFix
#define FT_Get_Advance tp_FT_Get_Advance
#define FT_Get_Advances tp_FT_Get_Advances
#define FT_Get_CMap_Format tp_FT_Get_CMap_Format
#define FT_Get_CMap_Language_ID tp_FT_Get_CMap_Language_ID
#define FT_Get_Char_Index tp_FT_Get_Char_Index
#define FT_Get_Charmap_Index tp_FT_Get_Charmap_Index
#define FT_Get_First_Char tp_FT_Get_First_Char
#define FT_Get_Font_Format tp_FT_Get_Font_Format
#define FT_Get_Glyph tp_FT_Get_Glyph
#define FT_Get_Glyph_Name tp_FT_Get_Glyph_Name
#define FT_Get_Kerning tp_FT_Get_Kerning
#define FT_Get_MM_Blend_Coordinates tp_FT_Get_MM_Blend_Coordinates
#define FT_Get_MM_Var tp_FT_Get_MM_Var
#define FT_Get_Module tp_FT_Get_Module
#define FT_Get_Module_Interface tp_FT_Get_Module_Interface
#define FT_Get_Multi_Master tp_FT_Get_Multi_Master
#define FT_Get_Name_Index tp_FT_Get_Name_Index
#define FT_Get_Next_Char tp_FT_Get_Next_Char
#define FT_Get_PS_Font_Info tp_FT_Get_PS_Font_Info
#define FT_Get_PS_Font_Private tp_FT_Get_PS_Font_Private
#define FT_Get_PS_Font_Value tp_FT_Get_PS_Font_Value
#define FT_Get_Postscript_Name tp_FT_Get_Postscript_Name
#define FT_Get_Renderer tp_FT_Get_Renderer
#define FT_Get_Sfnt_LangTag tp_FT_Get_Sfnt_LangTag
#define FT_Get_Sfnt_Name tp_FT_Get_Sfnt_Name
#define FT_Get_Sfnt_Name_Count tp_FT_Get_Sfnt_Name_Count
#define FT_Get_Sfnt_Table tp_FT_Get_Sfnt_Table
#define FT_Get_SubGlyph_Info tp_FT_Get_SubGlyph_Info
#define FT_Get_Track_Kerning tp_FT_Get_Track_Kerning
#define FT_Get_TrueType_Engine_Type tp_FT_Get_TrueType_Engine_Type
#define FT_Get_Var_Axis_Flags tp_FT_Get_Var_Axis_Flags
#define FT_Get_Var_Blend_Coordinates tp_FT_Get_Var_Blend_Coordinates
#define FT_Get_Var_Design_Coordinates tp_FT_Get_Var_Design_Coordinates
#define FT_Get_X11_Font_Format tp_FT_Get_X11_Font_Format
#define FT_GlyphLoader_Add tp_FT_GlyphLoader_Add
#define FT_GlyphLoader_CheckPoints tp_FT_GlyphLoader_CheckPoints
#define FT_GlyphLoader_CheckSubGlyphs tp_FT_GlyphLoader_CheckSubGlyphs
#define FT_GlyphLoader_CopyPoints tp_FT_GlyphLoader_CopyPoints
#define FT_GlyphLoader_CreateExtra tp_FT_GlyphLoader_CreateExtra
#define FT_GlyphLoader_Done tp_FT_GlyphLoader_Done
#define FT_GlyphLoader_New tp_FT_GlyphLoader_New
#define FT_GlyphLoader_Prepare tp_FT_GlyphLoader_Prepare
#define FT_GlyphLoader_Reset tp_FT_GlyphLoader_Reset
#define FT_GlyphLoader_Rewind tp_FT_GlyphLoader_Rewind
#define FT_GlyphSlot_Embolden tp_FT_GlyphSlot_Embolden
#define FT_GlyphSlot_Oblique tp_FT_GlyphSlot_Oblique
#define FT_GlyphSlot_Own_Bitmap tp_FT_GlyphSlot_Own_Bitmap
#define FT_Glyph_Copy tp_FT_Glyph_Copy
#define FT_Glyph_Get_CBox tp_FT_Glyph_Get_CBox
#define FT_Glyph_To_Bitmap tp_FT_Glyph_To_Bitmap
#define FT_Glyph_Transform tp_FT_Glyph_Transform
#define FT_Gzip_Uncompress tp_FT_Gzip_Uncompress
#define FT_Has_PS_Glyph_Names tp_FT_Has_PS_Glyph_Names
#define FT_Hypot tp_FT_Hypot
#define FT_Init_FreeType tp_FT_Init_FreeType
#define FT_Library_SetLcdFilter tp_FT_Library_SetLcdFilter
#define FT_Library_SetLcdFilterWeights tp_FT_Library_SetLcdFilterWeights
#define FT_Library_Version tp_FT_Library_Version
#define FT_List_Add tp_FT_List_Add
#define FT_List_Finalize tp_FT_List_Finalize
#define FT_List_Find tp_FT_List_Find
#define FT_List_Insert tp_FT_List_Insert
#define FT_List_Iterate tp_FT_List_Iterate
#define FT_List_Remove tp_FT_List_Remove
#define FT_List_Up tp_FT_List_Up
#define FT_Load_Char tp_FT_Load_Char
#define FT_Load_Glyph tp_FT_Load_Glyph
#define FT_Load_Sfnt_Table tp_FT_Load_Sfnt_Table
#define FT_Lookup_Renderer tp_FT_Lookup_Renderer
#define FT_MSB tp_FT_MSB
#define FT_Match_Size tp_FT_Match_Size
#define FT_Matrix_Invert tp_FT_Matrix_Invert
#define FT_Matrix_Multiply tp_FT_Matrix_Multiply
#define FT_Matrix_Multiply_Scaled tp_FT_Matrix_Multiply_Scaled
#define FT_MulDiv tp_FT_MulDiv
#define FT_MulDiv_No_Round tp_FT_MulDiv_No_Round
#define FT_MulFix tp_FT_MulFix
#define FT_New_Face tp_FT_New_Face
#define FT_New_GlyphSlot tp_FT_New_GlyphSlot
#define FT_New_Library tp_FT_New_Library
#define FT_New_Memory tp_FT_New_Memory
#define FT_New_Memory_Face tp_FT_New_Memory_Face
#define FT_New_Size tp_FT_New_Size
#define FT_Open_Face tp_FT_Open_Face
#define FT_Outline_Check tp_FT_Outline_Check
#define FT_Outline_Copy tp_FT_Outline_Copy
#define FT_Outline_Decompose tp_FT_Outline_Decompose
#define FT_Outline_Done tp_FT_Outline_Done
#define FT_Outline_Done_Internal tp_FT_Outline_Done_Internal
#define FT_Outline_Embolden tp_FT_Outline_Embolden
#define FT_Outline_EmboldenXY tp_FT_Outline_EmboldenXY
#define FT_Outline_Get_BBox tp_FT_Outline_Get_BBox
#define FT_Outline_Get_Bitmap tp_FT_Outline_Get_Bitmap
#define FT_Outline_Get_CBox tp_FT_Outline_Get_CBox
#define FT_Outline_Get_Orientation tp_FT_Outline_Get_Orientation
#define FT_Outline_New tp_FT_Outline_New
#define FT_Outline_New_Internal tp_FT_Outline_New_Internal
#define FT_Outline_Render tp_FT_Outline_Render
#define FT_Outline_Reverse tp_FT_Outline_Reverse
#define FT_Outline_Transform tp_FT_Outline_Transform
#define FT_Outline_Translate tp_FT_Outline_Translate
#define FT_Property_Get tp_FT_Property_Get
#define FT_Property_Set tp_FT_Property_Set
#define FT_Raccess_Get_DataOffsets tp_FT_Raccess_Get_DataOffsets
#define FT_Raccess_Get_HeaderInfo tp_FT_Raccess_Get_HeaderInfo
#define FT_Raccess_Guess tp_FT_Raccess_Guess
#define FT_Reference_Face tp_FT_Reference_Face
#define FT_Reference_Library tp_FT_Reference_Library
#define FT_Remove_Module tp_FT_Remove_Module
#define FT_Render_Glyph tp_FT_Render_Glyph
#define FT_Render_Glyph_Internal tp_FT_Render_Glyph_Internal
#define FT_Request_Metrics tp_FT_Request_Metrics
#define FT_Request_Size tp_FT_Request_Size
#define FT_RoundFix tp_FT_RoundFix
#define FT_Select_Charmap tp_FT_Select_Charmap
#define FT_Select_Metrics tp_FT_Select_Metrics
#define FT_Select_Size tp_FT_Select_Size
#define FT_Set_Char_Size tp_FT_Set_Char_Size
#define FT_Set_Charmap tp_FT_Set_Charmap
#define FT_Set_Debug_Hook tp_FT_Set_Debug_Hook
#define FT_Set_Default_Properties tp_FT_Set_Default_Properties
#define FT_Set_MM_Blend_Coordinates tp_FT_Set_MM_Blend_Coordinates
#define FT_Set_MM_Design_Coordinates tp_FT_Set_MM_Design_Coordinates
#define FT_Set_Pixel_Sizes tp_FT_Set_Pixel_Sizes
#define FT_Set_Renderer tp_FT_Set_Renderer
#define FT_Set_Transform tp_FT_Set_Transform
#define FT_Set_Var_Blend_Coordinates tp_FT_Set_Var_Blend_Coordinates
#define FT_Set_Var_Design_Coordinates tp_FT_Set_Var_Design_Coordinates
#define FT_Sfnt_Table_Info tp_FT_Sfnt_Table_Info
#define FT_Sin tp_FT_Sin
#define FT_Stream_Close tp_FT_Stream_Close
#define FT_Stream_EnterFrame tp_FT_Stream_EnterFrame
#define FT_Stream_ExitFrame tp_FT_Stream_ExitFrame
#define FT_Stream_ExtractFrame tp_FT_Stream_ExtractFrame
#define FT_Stream_Free tp_FT_Stream_Free
#define FT_Stream_GetChar tp_FT_Stream_GetChar
#define FT_Stream_GetULong tp_FT_Stream_GetULong
#define FT_Stream_GetULongLE tp_FT_Stream_GetULongLE
#define FT_Stream_GetUOffset tp_FT_Stream_GetUOffset
#define FT_Stream_GetUShort tp_FT_Stream_GetUShort
#define FT_Stream_GetUShortLE tp_FT_Stream_GetUShortLE
#define FT_Stream_New tp_FT_Stream_New
#define FT_Stream_Open tp_FT_Stream_Open
#define FT_Stream_OpenGzip tp_FT_Stream_OpenGzip
#define FT_Stream_OpenLZW tp_FT_Stream_OpenLZW
#define FT_Stream_OpenMemory tp_FT_Stream_OpenMemory
#define FT_Stream_Pos tp_FT_Stream_Pos
#define FT_Stream_Read tp_FT_Stream_Read
#define FT_Stream_ReadAt tp_FT_Stream_ReadAt
#define FT_Stream_ReadChar tp_FT_Stream_ReadChar
#define FT_Stream_ReadFields tp_FT_Stream_ReadFields
#define FT_Stream_ReadULong tp_FT_Stream_ReadULong
#define FT_Stream_ReadULongLE tp_FT_Stream_ReadULongLE
#define FT_Stream_ReadUOffset tp_FT_Stream_ReadUOffset
#define FT_Stream_ReadUShort tp_FT_Stream_ReadUShort
#define FT_Stream_ReadUShortLE tp_FT_Stream_ReadUShortLE
#define FT_Stream_ReleaseFrame tp_FT_Stream_ReleaseFrame
#define FT_Stream_Seek tp_FT_Stream_Seek
#define FT_Stream_Skip tp_FT_Stream_Skip
#define FT_Stream_TryRead tp_FT_Stream_TryRead
#define FT_Tan tp_FT_Tan
#define FT_Trace_Get_Count tp_FT_Trace_Get_Count
#define FT_Trace_Get_Name tp_FT_Trace_Get_Name
#define FT_Vector_From_Polar tp_FT_Vector_From_Polar
#define FT_Vector_Length tp_FT_Vector_Length
#define FT_Vector_NormLen tp_FT_Vector_NormLen
#define FT_Vector_Polarize tp_FT_Vector_Polarize
#define FT_Vector_Rotate tp_FT_Vector_Rotate
#define FT_Vector_Transform tp_FT_Vector_Transform
#define FT_Vector_Transform_Scaled tp_FT_Vector_Transform_Scaled
#define FT_Vector_Unit tp_FT_Vector_Unit
#define TT_New_Context tp_TT_New_Context
#define TT_RunIns tp_TT_RunIns
#define af_adlm_dflt_style_class tp_af_adlm_dflt_style_class
#define af_adlm_nonbase_uniranges tp_af_adlm_nonbase_uniranges
#define af_adlm_script_class tp_af_adlm_script_class
#define af_adlm_uniranges tp_af_adlm_uniranges
#define af_arab_dflt_style_class tp_af_arab_dflt_style_class
#define af_arab_nonbase_uniranges tp_af_arab_nonbase_uniranges
#define af_arab_script_class tp_af_arab_script_class
#define af_arab_uniranges tp_af_arab_uniranges
#define af_armn_dflt_style_class tp_af_armn_dflt_style_class
#define af_armn_nonbase_uniranges tp_af_armn_nonbase_uniranges
#define af_armn_script_class tp_af_armn_script_class
#define af_armn_uniranges tp_af_armn_uniranges
#define af_autofitter_interface tp_af_autofitter_interface
#define af_avst_dflt_style_class tp_af_avst_dflt_style_class
#define af_avst_nonbase_uniranges tp_af_avst_nonbase_uniranges
#define af_avst_script_class tp_af_avst_script_class
#define af_avst_uniranges tp_af_avst_uniranges
#define af_bamu_dflt_style_class tp_af_bamu_dflt_style_class
#define af_bamu_nonbase_uniranges tp_af_bamu_nonbase_uniranges
#define af_bamu_script_class tp_af_bamu_script_class
#define af_bamu_uniranges tp_af_bamu_uniranges
#define af_beng_dflt_style_class tp_af_beng_dflt_style_class
#define af_beng_nonbase_uniranges tp_af_beng_nonbase_uniranges
#define af_beng_script_class tp_af_beng_script_class
#define af_beng_uniranges tp_af_beng_uniranges
#define af_blue_strings tp_af_blue_strings
#define af_blue_stringsets tp_af_blue_stringsets
#define af_buhd_dflt_style_class tp_af_buhd_dflt_style_class
#define af_buhd_nonbase_uniranges tp_af_buhd_nonbase_uniranges
#define af_buhd_script_class tp_af_buhd_script_class
#define af_buhd_uniranges tp_af_buhd_uniranges
#define af_cakm_dflt_style_class tp_af_cakm_dflt_style_class
#define af_cakm_nonbase_uniranges tp_af_cakm_nonbase_uniranges
#define af_cakm_script_class tp_af_cakm_script_class
#define af_cakm_uniranges tp_af_cakm_uniranges
#define af_cans_dflt_style_class tp_af_cans_dflt_style_class
#define af_cans_nonbase_uniranges tp_af_cans_nonbase_uniranges
#define af_cans_script_class tp_af_cans_script_class
#define af_cans_uniranges tp_af_cans_uniranges
#define af_cari_dflt_style_class tp_af_cari_dflt_style_class
#define af_cari_nonbase_uniranges tp_af_cari_nonbase_uniranges
#define af_cari_script_class tp_af_cari_script_class
#define af_cari_uniranges tp_af_cari_uniranges
#define af_cher_dflt_style_class tp_af_cher_dflt_style_class
#define af_cher_nonbase_uniranges tp_af_cher_nonbase_uniranges
#define af_cher_script_class tp_af_cher_script_class
#define af_cher_uniranges tp_af_cher_uniranges
#define af_cjk_writing_system_class tp_af_cjk_writing_system_class
#define af_copt_dflt_style_class tp_af_copt_dflt_style_class
#define af_copt_nonbase_uniranges tp_af_copt_nonbase_uniranges
#define af_copt_script_class tp_af_copt_script_class
#define af_copt_uniranges tp_af_copt_uniranges
#define af_cprt_dflt_style_class tp_af_cprt_dflt_style_class
#define af_cprt_nonbase_uniranges tp_af_cprt_nonbase_uniranges
#define af_cprt_script_class tp_af_cprt_script_class
#define af_cprt_uniranges tp_af_cprt_uniranges
#define af_cyrl_c2cp_style_class tp_af_cyrl_c2cp_style_class
#define af_cyrl_c2sc_style_class tp_af_cyrl_c2sc_style_class
#define af_cyrl_dflt_style_class tp_af_cyrl_dflt_style_class
#define af_cyrl_nonbase_uniranges tp_af_cyrl_nonbase_uniranges
#define af_cyrl_ordn_style_class tp_af_cyrl_ordn_style_class
#define af_cyrl_pcap_style_class tp_af_cyrl_pcap_style_class
#define af_cyrl_script_class tp_af_cyrl_script_class
#define af_cyrl_sinf_style_class tp_af_cyrl_sinf_style_class
#define af_cyrl_smcp_style_class tp_af_cyrl_smcp_style_class
#define af_cyrl_subs_style_class tp_af_cyrl_subs_style_class
#define af_cyrl_sups_style_class tp_af_cyrl_sups_style_class
#define af_cyrl_titl_style_class tp_af_cyrl_titl_style_class
#define af_cyrl_uniranges tp_af_cyrl_uniranges
#define af_deva_dflt_style_class tp_af_deva_dflt_style_class
#define af_deva_nonbase_uniranges tp_af_deva_nonbase_uniranges
#define af_deva_script_class tp_af_deva_script_class
#define af_deva_uniranges tp_af_deva_uniranges
#define af_dsrt_dflt_style_class tp_af_dsrt_dflt_style_class
#define af_dsrt_nonbase_uniranges tp_af_dsrt_nonbase_uniranges
#define af_dsrt_script_class tp_af_dsrt_script_class
#define af_dsrt_uniranges tp_af_dsrt_uniranges
#define af_dummy_writing_system_class tp_af_dummy_writing_system_class
#define af_ethi_dflt_style_class tp_af_ethi_dflt_style_class
#define af_ethi_nonbase_uniranges tp_af_ethi_nonbase_uniranges
#define af_ethi_script_class tp_af_ethi_script_class
#define af_ethi_uniranges tp_af_ethi_uniranges
#define af_geok_dflt_style_class tp_af_geok_dflt_style_class
#define af_geok_nonbase_uniranges tp_af_geok_nonbase_uniranges
#define af_geok_script_class tp_af_geok_script_class
#define af_geok_uniranges tp_af_geok_uniranges
#define af_geor_dflt_style_class tp_af_geor_dflt_style_class
#define af_geor_nonbase_uniranges tp_af_geor_nonbase_uniranges
#define af_geor_script_class tp_af_geor_script_class
#define af_geor_uniranges tp_af_geor_uniranges
#define af_glag_dflt_style_class tp_af_glag_dflt_style_class
#define af_glag_nonbase_uniranges tp_af_glag_nonbase_uniranges
#define af_glag_script_class tp_af_glag_script_class
#define af_glag_uniranges tp_af_glag_uniranges
#define af_goth_dflt_style_class tp_af_goth_dflt_style_class
#define af_goth_nonbase_uniranges tp_af_goth_nonbase_uniranges
#define af_goth_script_class tp_af_goth_script_class
#define af_goth_uniranges tp_af_goth_uniranges
#define af_grek_c2cp_style_class tp_af_grek_c2cp_style_class
#define af_grek_c2sc_style_class tp_af_grek_c2sc_style_class
#define af_grek_dflt_style_class tp_af_grek_dflt_style_class
#define af_grek_nonbase_uniranges tp_af_grek_nonbase_uniranges
#define af_grek_ordn_style_class tp_af_grek_ordn_style_class
#define af_grek_pcap_style_class tp_af_grek_pcap_style_class
#define af_grek_script_class tp_af_grek_script_class
#define af_grek_sinf_style_class tp_af_grek_sinf_style_class
#define af_grek_smcp_style_class tp_af_grek_smcp_style_class
#define af_grek_subs_style_class tp_af_grek_subs_style_class
#define af_grek_sups_style_class tp_af_grek_sups_style_class
#define af_grek_titl_style_class tp_af_grek_titl_style_class
#define af_grek_uniranges tp_af_grek_uniranges
#define af_gujr_dflt_style_class tp_af_gujr_dflt_style_class
#define af_gujr_nonbase_uniranges tp_af_gujr_nonbase_uniranges
#define af_gujr_script_class tp_af_gujr_script_class
#define af_gujr_uniranges tp_af_gujr_uniranges
#define af_guru_dflt_style_class tp_af_guru_dflt_style_class
#define af_guru_nonbase_uniranges tp_af_guru_nonbase_uniranges
#define af_guru_script_class tp_af_guru_script_class
#define af_guru_uniranges tp_af_guru_uniranges
#define af_hani_dflt_style_class tp_af_hani_dflt_style_class
#define af_hani_nonbase_uniranges tp_af_hani_nonbase_uniranges
#define af_hani_script_class tp_af_hani_script_class
#define af_hani_uniranges tp_af_hani_uniranges
#define af_hebr_dflt_style_class tp_af_hebr_dflt_style_class
#define af_hebr_nonbase_uniranges tp_af_hebr_nonbase_uniranges
#define af_hebr_script_class tp_af_hebr_script_class
#define af_hebr_uniranges tp_af_hebr_uniranges
#define af_indic_writing_system_class tp_af_indic_writing_system_class
#define af_kali_dflt_style_class tp_af_kali_dflt_style_class
#define af_kali_nonbase_uniranges tp_af_kali_nonbase_uniranges
#define af_kali_script_class tp_af_kali_script_class
#define af_kali_uniranges tp_af_kali_uniranges
#define af_khmr_dflt_style_class tp_af_khmr_dflt_style_class
#define af_khmr_nonbase_uniranges tp_af_khmr_nonbase_uniranges
#define af_khmr_script_class tp_af_khmr_script_class
#define af_khmr_uniranges tp_af_khmr_uniranges
#define af_khms_dflt_style_class tp_af_khms_dflt_style_class
#define af_khms_nonbase_uniranges tp_af_khms_nonbase_uniranges
#define af_khms_script_class tp_af_khms_script_class
#define af_khms_uniranges tp_af_khms_uniranges
#define af_knda_dflt_style_class tp_af_knda_dflt_style_class
#define af_knda_nonbase_uniranges tp_af_knda_nonbase_uniranges
#define af_knda_script_class tp_af_knda_script_class
#define af_knda_uniranges tp_af_knda_uniranges
#define af_lao_dflt_style_class tp_af_lao_dflt_style_class
#define af_lao_nonbase_uniranges tp_af_lao_nonbase_uniranges
#define af_lao_script_class tp_af_lao_script_class
#define af_lao_uniranges tp_af_lao_uniranges
#define af_latb_dflt_style_class tp_af_latb_dflt_style_class
#define af_latb_nonbase_uniranges tp_af_latb_nonbase_uniranges
#define af_latb_script_class tp_af_latb_script_class
#define af_latb_uniranges tp_af_latb_uniranges
#define af_latin_writing_system_class tp_af_latin_writing_system_class
#define af_latn_c2cp_style_class tp_af_latn_c2cp_style_class
#define af_latn_c2sc_style_class tp_af_latn_c2sc_style_class
#define af_latn_dflt_style_class tp_af_latn_dflt_style_class
#define af_latn_nonbase_uniranges tp_af_latn_nonbase_uniranges
#define af_latn_ordn_style_class tp_af_latn_ordn_style_class
#define af_latn_pcap_style_class tp_af_latn_pcap_style_class
#define af_latn_script_class tp_af_latn_script_class
#define af_latn_sinf_style_class tp_af_latn_sinf_style_class
#define af_latn_smcp_style_class tp_af_latn_smcp_style_class
#define af_latn_subs_style_class tp_af_latn_subs_style_class
#define af_latn_sups_style_class tp_af_latn_sups_style_class
#define af_latn_titl_style_class tp_af_latn_titl_style_class
#define af_latn_uniranges tp_af_latn_uniranges
#define af_latp_dflt_style_class tp_af_latp_dflt_style_class
#define af_latp_nonbase_uniranges tp_af_latp_nonbase_uniranges
#define af_latp_script_class tp_af_latp_script_class
#define af_latp_uniranges tp_af_latp_uniranges
#define af_limb_dflt_style_class tp_af_limb_dflt_style_class
#define af_limb_nonbase_uniranges tp_af_limb_nonbase_uniranges
#define af_limb_script_class tp_af_limb_script_class
#define af_limb_uniranges tp_af_limb_uniranges
#define af_lisu_dflt_style_class tp_af_lisu_dflt_style_class
#define af_lisu_nonbase_uniranges tp_af_lisu_nonbase_uniranges
#define af_lisu_script_class tp_af_lisu_script_class
#define af_lisu_uniranges tp_af_lisu_uniranges
#define af_mlym_dflt_style_class tp_af_mlym_dflt_style_class
#define af_mlym_nonbase_uniranges tp_af_mlym_nonbase_uniranges
#define af_mlym_script_class tp_af_mlym_script_class
#define af_mlym_uniranges tp_af_mlym_uniranges
#define af_mymr_dflt_style_class tp_af_mymr_dflt_style_class
#define af_mymr_nonbase_uniranges tp_af_mymr_nonbase_uniranges
#define af_mymr_script_class tp_af_mymr_script_class
#define af_mymr_uniranges tp_af_mymr_uniranges
#define af_nkoo_dflt_style_class tp_af_nkoo_dflt_style_class
#define af_nkoo_nonbase_uniranges tp_af_nkoo_nonbase_uniranges
#define af_nkoo_script_class tp_af_nkoo_script_class
#define af_nkoo_uniranges tp_af_nkoo_uniranges
#define af_none_dflt_style_class tp_af_none_dflt_style_class
#define af_none_nonbase_uniranges tp_af_none_nonbase_uniranges
#define af_none_script_class tp_af_none_script_class
#define af_none_uniranges tp_af_none_uniranges
#define af_olck_dflt_style_class tp_af_olck_dflt_style_class
#define af_olck_nonbase_uniranges tp_af_olck_nonbase_uniranges
#define af_olck_script_class tp_af_olck_script_class
#define af_olck_uniranges tp_af_olck_uniranges
#define af_orkh_dflt_style_class tp_af_orkh_dflt_style_class
#define af_orkh_nonbase_uniranges tp_af_orkh_nonbase_uniranges
#define af_orkh_script_class tp_af_orkh_script_class
#define af_orkh_uniranges tp_af_orkh_uniranges
#define af_orya_dflt_style_class tp_af_orya_dflt_style_class
#define af_orya_nonbase_uniranges tp_af_orya_nonbase_uniranges
#define af_orya_script_class tp_af_orya_script_class
#define af_orya_uniranges tp_af_orya_uniranges
#define af_osge_dflt_style_class tp_af_osge_dflt_style_class
#define af_osge_nonbase_uniranges tp_af_osge_nonbase_uniranges
#define af_osge_script_class tp_af_osge_script_class
#define af_osge_uniranges tp_af_osge_uniranges
#define af_osma_dflt_style_class tp_af_osma_dflt_style_class
#define af_osma_nonbase_uniranges tp_af_osma_nonbase_uniranges
#define af_osma_script_class tp_af_osma_script_class
#define af_osma_uniranges tp_af_osma_uniranges
#define af_saur_dflt_style_class tp_af_saur_dflt_style_class
#define af_saur_nonbase_uniranges tp_af_saur_nonbase_uniranges
#define af_saur_script_class tp_af_saur_script_class
#define af_saur_uniranges tp_af_saur_uniranges
#define af_script_classes tp_af_script_classes
#define af_shaper_buf_create tp_af_shaper_buf_create
#define af_shaper_buf_destroy tp_af_shaper_buf_destroy
#define af_shaper_get_cluster tp_af_shaper_get_cluster
#define af_shaper_get_coverage tp_af_shaper_get_coverage
#define af_shaper_get_elem tp_af_shaper_get_elem
#define af_shaw_dflt_style_class tp_af_shaw_dflt_style_class
#define af_shaw_nonbase_uniranges tp_af_shaw_nonbase_uniranges
#define af_shaw_script_class tp_af_shaw_script_class
#define af_shaw_uniranges tp_af_shaw_uniranges
#define af_sinh_dflt_style_class tp_af_sinh_dflt_style_class
#define af_sinh_nonbase_uniranges tp_af_sinh_nonbase_uniranges
#define af_sinh_script_class tp_af_sinh_script_class
#define af_sinh_uniranges tp_af_sinh_uniranges
#define af_style_classes tp_af_style_classes
#define af_sund_dflt_style_class tp_af_sund_dflt_style_class
#define af_sund_nonbase_uniranges tp_af_sund_nonbase_uniranges
#define af_sund_script_class tp_af_sund_script_class
#define af_sund_uniranges tp_af_sund_uniranges
#define af_sylo_dflt_style_class tp_af_sylo_dflt_style_class
#define af_sylo_nonbase_uniranges tp_af_sylo_nonbase_uniranges
#define af_sylo_script_class tp_af_sylo_script_class
#define af_sylo_uniranges tp_af_sylo_uniranges
#define af_taml_dflt_style_class tp_af_taml_dflt_style_class
#define af_taml_nonbase_uniranges tp_af_taml_nonbase_uniranges
#define af_taml_script_class tp_af_taml_script_class
#define af_taml_uniranges tp_af_taml_uniranges
#define af_tavt_dflt_style_class tp_af_tavt_dflt_style_class
#define af_tavt_nonbase_uniranges tp_af_tavt_nonbase_uniranges
#define af_tavt_script_class tp_af_tavt_script_class
#define af_tavt_uniranges tp_af_tavt_uniranges
#define af_telu_dflt_style_class tp_af_telu_dflt_style_class
#define af_telu_nonbase_uniranges tp_af_telu_nonbase_uniranges
#define af_telu_script_class tp_af_telu_script_class
#define af_telu_uniranges tp_af_telu_uniranges
#define af_tfng_dflt_style_class tp_af_tfng_dflt_style_class
#define af_tfng_nonbase_uniranges tp_af_tfng_nonbase_uniranges
#define af_tfng_script_class tp_af_tfng_script_class
#define af_tfng_uniranges tp_af_tfng_uniranges
#define af_thai_dflt_style_class tp_af_thai_dflt_style_class
#define af_thai_nonbase_uniranges tp_af_thai_nonbase_uniranges
#define af_thai_script_class tp_af_thai_script_class
#define af_thai_uniranges tp_af_thai_uniranges
#define af_tibt_dflt_style_class tp_af_tibt_dflt_style_class
#define af_tibt_nonbase_uniranges tp_af_tibt_nonbase_uniranges
#define af_tibt_script_class tp_af_tibt_script_class
#define af_tibt_uniranges tp_af_tibt_uniranges
#define af_vaii_dflt_style_class tp_af_vaii_dflt_style_class
#define af_vaii_nonbase_uniranges tp_af_vaii_nonbase_uniranges
#define af_vaii_script_class tp_af_vaii_script_class
#define af_vaii_uniranges tp_af_vaii_uniranges
#define af_writing_system_classes tp_af_writing_system_classes
#define afm_parser_funcs tp_afm_parser_funcs
#define autofit_module_class tp_autofit_module_class
#define bdf_driver_class tp_bdf_driver_class
#define cff_cmap_encoding_class_rec tp_cff_cmap_encoding_class_rec
#define cff_cmap_unicode_class_rec tp_cff_cmap_unicode_class_rec
#define cff_driver_class tp_cff_driver_class
#define ft_adobe_glyph_list tp_ft_adobe_glyph_list
#define ft_bitmap_glyph_class tp_ft_bitmap_glyph_class
#define ft_corner_is_flat tp_ft_corner_is_flat
#define ft_corner_orientation tp_ft_corner_orientation
#define ft_debug_init tp_ft_debug_init
#define ft_glyphslot_alloc_bitmap tp_ft_glyphslot_alloc_bitmap
#define ft_glyphslot_free_bitmap tp_ft_glyphslot_free_bitmap
#define ft_glyphslot_set_bitmap tp_ft_glyphslot_set_bitmap
#define ft_grays_raster tp_ft_grays_raster
#define ft_hash_num_init tp_ft_hash_num_init
#define ft_hash_num_insert tp_ft_hash_num_insert
#define ft_hash_num_lookup tp_ft_hash_num_lookup
#define ft_hash_str_free tp_ft_hash_str_free
#define ft_hash_str_init tp_ft_hash_str_init
#define ft_hash_str_insert tp_ft_hash_str_insert
#define ft_hash_str_lookup tp_ft_hash_str_lookup
#define ft_lcd_filter_fir tp_ft_lcd_filter_fir
#define ft_lzwstate_done tp_ft_lzwstate_done
#define ft_lzwstate_init tp_ft_lzwstate_init
#define ft_lzwstate_io tp_ft_lzwstate_io
#define ft_lzwstate_reset tp_ft_lzwstate_reset
#define ft_mac_names tp_ft_mac_names
#define ft_mem_alloc tp_ft_mem_alloc
#define ft_mem_dup tp_ft_mem_dup
#define ft_mem_free tp_ft_mem_free
#define ft_mem_qalloc tp_ft_mem_qalloc
#define ft_mem_qrealloc tp_ft_mem_qrealloc
#define ft_mem_realloc tp_ft_mem_realloc
#define ft_mem_strcpyn tp_ft_mem_strcpyn
#define ft_mem_strdup tp_ft_mem_strdup
#define ft_module_get_service tp_ft_module_get_service
#define ft_outline_glyph_class tp_ft_outline_glyph_class
#define ft_property_string_set tp_ft_property_string_set
#define ft_raster1_renderer_class tp_ft_raster1_renderer_class
#define ft_service_list_lookup tp_ft_service_list_lookup
#define ft_sid_names tp_ft_sid_names
#define ft_smooth_lcd_renderer_class tp_ft_smooth_lcd_renderer_class
#define ft_smooth_lcdv_renderer_class tp_ft_smooth_lcdv_renderer_class
#define ft_smooth_renderer_class tp_ft_smooth_renderer_class
#define ft_standard_glyph_names tp_ft_standard_glyph_names
#define ft_standard_raster tp_ft_standard_raster
#define ft_synthesize_vertical_metrics tp_ft_synthesize_vertical_metrics
#define ft_validator_error tp_ft_validator_error
#define ft_validator_init tp_ft_validator_init
#define ft_validator_run tp_ft_validator_run
#define otv_module_class tp_otv_module_class
#define pcf_driver_class tp_pcf_driver_class
#define pfr_cmap_class_rec tp_pfr_cmap_class_rec
#define pfr_driver_class tp_pfr_driver_class
#define ps_hints_apply tp_ps_hints_apply
#define ps_parser_funcs tp_ps_parser_funcs
#define ps_table_funcs tp_ps_table_funcs
#define psaux_module_class tp_psaux_module_class
#define pshinter_module_class tp_pshinter_module_class
#define psnames_module_class tp_psnames_module_class
#define sfnt_module_class tp_sfnt_module_class
#define t1_builder_funcs tp_t1_builder_funcs
#define t1_cmap_classes tp_t1_cmap_classes
#define t1_cmap_custom_class_rec tp_t1_cmap_custom_class_rec
#define t1_cmap_expert_class_rec tp_t1_cmap_expert_class_rec
#define t1_cmap_standard_class_rec tp_t1_cmap_standard_class_rec
#define t1_cmap_unicode_class_rec tp_t1_cmap_unicode_class_rec
#define t1_decoder_funcs tp_t1_decoder_funcs
#define t1_driver_class tp_t1_driver_class
#define t1_expert_encoding tp_t1_expert_encoding
#define t1_standard_encoding tp_t1_standard_encoding
#define t1cid_driver_class tp_t1cid_driver_class
#define t42_driver_class tp_t42_driver_class
#define tt_cmap0_class_rec tp_tt_cmap0_class_rec
#define tt_cmap10_class_rec tp_tt_cmap10_class_rec
#define tt_cmap12_class_rec tp_tt_cmap12_class_rec
#define tt_cmap13_class_rec tp_tt_cmap13_class_rec
#define tt_cmap14_class_rec tp_tt_cmap14_class_rec
#define tt_cmap2_class_rec tp_tt_cmap2_class_rec
#define tt_cmap4_class_rec tp_tt_cmap4_class_rec
#define tt_cmap6_class_rec tp_tt_cmap6_class_rec
#define tt_cmap8_class_rec tp_tt_cmap8_class_rec
#define tt_cmap_unicode_class_rec tp_tt_cmap_unicode_class_rec
#define tt_default_graphics_state tp_tt_default_graphics_state
#define tt_driver_class tp_tt_driver_class
#define winfnt_driver_class tp_winfnt_driver_class

#endif /* FT_PREFIX_H_INCLUDED */

