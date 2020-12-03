<!DOCTYPE html><html lang="en"><head><meta charset="utf-8"><title>libvpx/vp9/common/vp9_tile_common.c - platform/external/libvpx - Git at Google</title><link rel="stylesheet" type="text/css" href="/+static/base.YxgkRP5in5Amk_r7PKOxxA.cache.css"><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.AOMOBqJIPcDq491E2ExAAw.cache.css"><!-- default customHeadTagPart --></head><body class="Site"><header class="Site-header"><div class="Header"><a class="Header-image" href="/"><img src="//www.gstatic.com/images/branding/lockups/2x/lockup_git_color_108x24dp.png" width="108" height="24" alt="Google Git"></a><div class="Header-menu"> <a class="Header-menuItem" href="https://accounts.google.com/AccountChooser?service=gerritcodereview&amp;continue=https://android.googlesource.com/login/platform/external/libvpx/%2B/5a9753fca56f0eeb9f61e342b2fccffc364f9426/libvpx/vp9/common/vp9_tile_common.c">Sign in</a> </div></div></header><div class="Site-content"><div class="Container "><div class="Breadcrumbs"><a class="Breadcrumbs-crumb" href="/?format=HTML">android</a> / <a class="Breadcrumbs-crumb" href="/platform/">platform</a> / <a class="Breadcrumbs-crumb" href="/platform/external/">external</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/">libvpx</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/5a9753fca56f0eeb9f61e342b2fccffc364f9426">5a9753fca56f0eeb9f61e342b2fccffc364f9426</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/5a9753fca56f0eeb9f61e342b2fccffc364f9426/">.</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/5a9753fca56f0eeb9f61e342b2fccffc364f9426/libvpx">libvpx</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/5a9753fca56f0eeb9f61e342b2fccffc364f9426/libvpx/vp9">vp9</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/5a9753fca56f0eeb9f61e342b2fccffc364f9426/libvpx/vp9/common">common</a> / <span class="Breadcrumbs-crumb">vp9_tile_common.c</span></div><div class="u-sha1 u-monospace BlobSha1">blob: 7a20e0a9e7337b6042acff4fde7c414e929b6fa3 [<a href="/platform/external/libvpx/+/5a9753fca56f0eeb9f61e342b2fccffc364f9426/libvpx/vp9/common/vp9_tile_common.c">file</a>] [<a href="/platform/external/libvpx/+log/5a9753fca56f0eeb9f61e342b2fccffc364f9426/libvpx/vp9/common/vp9_tile_common.c">log</a>] [<a href="/platform/external/libvpx/+blame/5a9753fca56f0eeb9f61e342b2fccffc364f9426/libvpx/vp9/common/vp9_tile_common.c">blame</a>]</div><table class="FileContents"><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="1" onclick="window.location.hash='#1'"></td><td class="FileContents-lineContents" id="1"><span class="com">/*</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="2" onclick="window.location.hash='#2'"></td><td class="FileContents-lineContents" id="2"><span class="com"> *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="3" onclick="window.location.hash='#3'"></td><td class="FileContents-lineContents" id="3"><span class="com"> *</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="4" onclick="window.location.hash='#4'"></td><td class="FileContents-lineContents" id="4"><span class="com"> *  Use of this source code is governed by a BSD-style license</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="5" onclick="window.location.hash='#5'"></td><td class="FileContents-lineContents" id="5"><span class="com"> *  that can be found in the LICENSE file in the root of the source</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="6" onclick="window.location.hash='#6'"></td><td class="FileContents-lineContents" id="6"><span class="com"> *  tree. An additional intellectual property rights grant can be found</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="7" onclick="window.location.hash='#7'"></td><td class="FileContents-lineContents" id="7"><span class="com"> *  in the file PATENTS.  All contributing project authors may</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="8" onclick="window.location.hash='#8'"></td><td class="FileContents-lineContents" id="8"><span class="com"> *  be found in the AUTHORS file in the root of the source tree.</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="9" onclick="window.location.hash='#9'"></td><td class="FileContents-lineContents" id="9"><span class="com"> */</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="10" onclick="window.location.hash='#10'"></td><td class="FileContents-lineContents" id="10"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="11" onclick="window.location.hash='#11'"></td><td class="FileContents-lineContents" id="11"><span class="com">#include</span><span class="pln"> </span><span class="str">&quot;vp9/common/vp9_tile_common.h&quot;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="12" onclick="window.location.hash='#12'"></td><td class="FileContents-lineContents" id="12"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="13" onclick="window.location.hash='#13'"></td><td class="FileContents-lineContents" id="13"><span class="com">#include</span><span class="pln"> </span><span class="str">&quot;vp9/common/vp9_onyxc_int.h&quot;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="14" onclick="window.location.hash='#14'"></td><td class="FileContents-lineContents" id="14"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="15" onclick="window.location.hash='#15'"></td><td class="FileContents-lineContents" id="15"><span class="com">#define</span><span class="pln"> MIN_TILE_WIDTH_B64 </span><span class="lit">4</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="16" onclick="window.location.hash='#16'"></td><td class="FileContents-lineContents" id="16"><span class="com">#define</span><span class="pln"> MAX_TILE_WIDTH_B64 </span><span class="lit">64</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="17" onclick="window.location.hash='#17'"></td><td class="FileContents-lineContents" id="17"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="18" onclick="window.location.hash='#18'"></td><td class="FileContents-lineContents" id="18"><span class="kwd">static</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> get_tile_offset</span><span class="pun">(</span><span class="typ">int</span><span class="pln"> idx</span><span class="pun">,</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> mis</span><span class="pun">,</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> log2</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="19" onclick="window.location.hash='#19'"></td><td class="FileContents-lineContents" id="19"><span class="pln">  </span><span class="kwd">const</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> sb_cols </span><span class="pun">=</span><span class="pln"> mi_cols_aligned_to_sb</span><span class="pun">(</span><span class="pln">mis</span><span class="pun">)</span><span class="pln"> </span><span class="pun">&gt;&gt;</span><span class="pln"> MI_BLOCK_SIZE_LOG2</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="20" onclick="window.location.hash='#20'"></td><td class="FileContents-lineContents" id="20"><span class="pln">  </span><span class="kwd">const</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> offset </span><span class="pun">=</span><span class="pln"> </span><span class="pun">((</span><span class="pln">idx </span><span class="pun">*</span><span class="pln"> sb_cols</span><span class="pun">)</span><span class="pln"> </span><span class="pun">&gt;&gt;</span><span class="pln"> log2</span><span class="pun">)</span><span class="pln"> </span><span class="pun">&lt;&lt;</span><span class="pln"> MI_BLOCK_SIZE_LOG2</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="21" onclick="window.location.hash='#21'"></td><td class="FileContents-lineContents" id="21"><span class="pln">  </span><span class="kwd">return</span><span class="pln"> MIN</span><span class="pun">(</span><span class="pln">offset</span><span class="pun">,</span><span class="pln"> mis</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="22" onclick="window.location.hash='#22'"></td><td class="FileContents-lineContents" id="22"><span class="pun">}</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="23" onclick="window.location.hash='#23'"></td><td class="FileContents-lineContents" id="23"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="24" onclick="window.location.hash='#24'"></td><td class="FileContents-lineContents" id="24"><span class="kwd">void</span><span class="pln"> vp9_tile_set_row</span><span class="pun">(</span><span class="typ">TileInfo</span><span class="pln"> </span><span class="pun">*</span><span class="pln">tile</span><span class="pun">,</span><span class="pln"> </span><span class="kwd">const</span><span class="pln"> VP9_COMMON </span><span class="pun">*</span><span class="pln">cm</span><span class="pun">,</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> row</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="25" onclick="window.location.hash='#25'"></td><td class="FileContents-lineContents" id="25"><span class="pln">  tile</span><span class="pun">-&gt;</span><span class="pln">mi_row_start </span><span class="pun">=</span><span class="pln"> get_tile_offset</span><span class="pun">(</span><span class="pln">row</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">-&gt;</span><span class="pln">mi_rows</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">-&gt;</span><span class="pln">log2_tile_rows</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="26" onclick="window.location.hash='#26'"></td><td class="FileContents-lineContents" id="26"><span class="pln">  tile</span><span class="pun">-&gt;</span><span class="pln">mi_row_end </span><span class="pun">=</span><span class="pln"> get_tile_offset</span><span class="pun">(</span><span class="pln">row </span><span class="pun">+</span><span class="pln"> </span><span class="lit">1</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">-&gt;</span><span class="pln">mi_rows</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">-&gt;</span><span class="pln">log2_tile_rows</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="27" onclick="window.location.hash='#27'"></td><td class="FileContents-lineContents" id="27"><span class="pun">}</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="28" onclick="window.location.hash='#28'"></td><td class="FileContents-lineContents" id="28"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="29" onclick="window.location.hash='#29'"></td><td class="FileContents-lineContents" id="29"><span class="kwd">void</span><span class="pln"> vp9_tile_set_col</span><span class="pun">(</span><span class="typ">TileInfo</span><span class="pln"> </span><span class="pun">*</span><span class="pln">tile</span><span class="pun">,</span><span class="pln"> </span><span class="kwd">const</span><span class="pln"> VP9_COMMON </span><span class="pun">*</span><span class="pln">cm</span><span class="pun">,</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> col</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="30" onclick="window.location.hash='#30'"></td><td class="FileContents-lineContents" id="30"><span class="pln">  tile</span><span class="pun">-&gt;</span><span class="pln">mi_col_start </span><span class="pun">=</span><span class="pln"> get_tile_offset</span><span class="pun">(</span><span class="pln">col</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">-&gt;</span><span class="pln">mi_cols</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">-&gt;</span><span class="pln">log2_tile_cols</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="31" onclick="window.location.hash='#31'"></td><td class="FileContents-lineContents" id="31"><span class="pln">  tile</span><span class="pun">-&gt;</span><span class="pln">mi_col_end </span><span class="pun">=</span><span class="pln"> get_tile_offset</span><span class="pun">(</span><span class="pln">col </span><span class="pun">+</span><span class="pln"> </span><span class="lit">1</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">-&gt;</span><span class="pln">mi_cols</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">-&gt;</span><span class="pln">log2_tile_cols</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="32" onclick="window.location.hash='#32'"></td><td class="FileContents-lineContents" id="32"><span class="pun">}</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="33" onclick="window.location.hash='#33'"></td><td class="FileContents-lineContents" id="33"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="34" onclick="window.location.hash='#34'"></td><td class="FileContents-lineContents" id="34"><span class="kwd">void</span><span class="pln"> vp9_tile_init</span><span class="pun">(</span><span class="typ">TileInfo</span><span class="pln"> </span><span class="pun">*</span><span class="pln">tile</span><span class="pun">,</span><span class="pln"> </span><span class="kwd">const</span><span class="pln"> VP9_COMMON </span><span class="pun">*</span><span class="pln">cm</span><span class="pun">,</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> row</span><span class="pun">,</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> col</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="35" onclick="window.location.hash='#35'"></td><td class="FileContents-lineContents" id="35"><span class="pln">  vp9_tile_set_row</span><span class="pun">(</span><span class="pln">tile</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">,</span><span class="pln"> row</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="36" onclick="window.location.hash='#36'"></td><td class="FileContents-lineContents" id="36"><span class="pln">  vp9_tile_set_col</span><span class="pun">(</span><span class="pln">tile</span><span class="pun">,</span><span class="pln"> cm</span><span class="pun">,</span><span class="pln"> col</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="37" onclick="window.location.hash='#37'"></td><td class="FileContents-lineContents" id="37"><span class="pun">}</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="38" onclick="window.location.hash='#38'"></td><td class="FileContents-lineContents" id="38"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="39" onclick="window.location.hash='#39'"></td><td class="FileContents-lineContents" id="39"><span class="kwd">static</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> get_min_log2_tile_cols</span><span class="pun">(</span><span class="kwd">const</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> sb64_cols</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="40" onclick="window.location.hash='#40'"></td><td class="FileContents-lineContents" id="40"><span class="pln">  </span><span class="typ">int</span><span class="pln"> min_log2 </span><span class="pun">=</span><span class="pln"> </span><span class="lit">0</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="41" onclick="window.location.hash='#41'"></td><td class="FileContents-lineContents" id="41"><span class="pln">  </span><span class="kwd">while</span><span class="pln"> </span><span class="pun">((</span><span class="pln">MAX_TILE_WIDTH_B64 </span><span class="pun">&lt;&lt;</span><span class="pln"> min_log2</span><span class="pun">)</span><span class="pln"> </span><span class="pun">&lt;</span><span class="pln"> sb64_cols</span><span class="pun">)</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="42" onclick="window.location.hash='#42'"></td><td class="FileContents-lineContents" id="42"><span class="pln">    </span><span class="pun">++</span><span class="pln">min_log2</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="43" onclick="window.location.hash='#43'"></td><td class="FileContents-lineContents" id="43"><span class="pln">  </span><span class="kwd">return</span><span class="pln"> min_log2</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="44" onclick="window.location.hash='#44'"></td><td class="FileContents-lineContents" id="44"><span class="pun">}</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="45" onclick="window.location.hash='#45'"></td><td class="FileContents-lineContents" id="45"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="46" onclick="window.location.hash='#46'"></td><td class="FileContents-lineContents" id="46"><span class="kwd">static</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> get_max_log2_tile_cols</span><span class="pun">(</span><span class="kwd">const</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> sb64_cols</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="47" onclick="window.location.hash='#47'"></td><td class="FileContents-lineContents" id="47"><span class="pln">  </span><span class="typ">int</span><span class="pln"> max_log2 </span><span class="pun">=</span><span class="pln"> </span><span class="lit">1</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="48" onclick="window.location.hash='#48'"></td><td class="FileContents-lineContents" id="48"><span class="pln">  </span><span class="kwd">while</span><span class="pln"> </span><span class="pun">((</span><span class="pln">sb64_cols </span><span class="pun">&gt;&gt;</span><span class="pln"> max_log2</span><span class="pun">)</span><span class="pln"> </span><span class="pun">&gt;=</span><span class="pln"> MIN_TILE_WIDTH_B64</span><span class="pun">)</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="49" onclick="window.location.hash='#49'"></td><td class="FileContents-lineContents" id="49"><span class="pln">    </span><span class="pun">++</span><span class="pln">max_log2</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="50" onclick="window.location.hash='#50'"></td><td class="FileContents-lineContents" id="50"><span class="pln">  </span><span class="kwd">return</span><span class="pln"> max_log2 </span><span class="pun">-</span><span class="pln"> </span><span class="lit">1</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="51" onclick="window.location.hash='#51'"></td><td class="FileContents-lineContents" id="51"><span class="pun">}</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="52" onclick="window.location.hash='#52'"></td><td class="FileContents-lineContents" id="52"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="53" onclick="window.location.hash='#53'"></td><td class="FileContents-lineContents" id="53"><span class="kwd">void</span><span class="pln"> vp9_get_tile_n_bits</span><span class="pun">(</span><span class="typ">int</span><span class="pln"> mi_cols</span><span class="pun">,</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="54" onclick="window.location.hash='#54'"></td><td class="FileContents-lineContents" id="54"><span class="pln">                         </span><span class="typ">int</span><span class="pln"> </span><span class="pun">*</span><span class="pln">min_log2_tile_cols</span><span class="pun">,</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> </span><span class="pun">*</span><span class="pln">max_log2_tile_cols</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="55" onclick="window.location.hash='#55'"></td><td class="FileContents-lineContents" id="55"><span class="pln">  </span><span class="kwd">const</span><span class="pln"> </span><span class="typ">int</span><span class="pln"> sb64_cols </span><span class="pun">=</span><span class="pln"> mi_cols_aligned_to_sb</span><span class="pun">(</span><span class="pln">mi_cols</span><span class="pun">)</span><span class="pln"> </span><span class="pun">&gt;&gt;</span><span class="pln"> MI_BLOCK_SIZE_LOG2</span><span class="pun">;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="56" onclick="window.location.hash='#56'"></td><td class="FileContents-lineContents" id="56"><span class="pln">  </span><span class="pun">*</span><span class="pln">min_log2_tile_cols </span><span class="pun">=</span><span class="pln"> get_min_log2_tile_cols</span><span class="pun">(</span><span class="pln">sb64_cols</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="57" onclick="window.location.hash='#57'"></td><td class="FileContents-lineContents" id="57"><span class="pln">  </span><span class="pun">*</span><span class="pln">max_log2_tile_cols </span><span class="pun">=</span><span class="pln"> get_max_log2_tile_cols</span><span class="pun">(</span><span class="pln">sb64_cols</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="58" onclick="window.location.hash='#58'"></td><td class="FileContents-lineContents" id="58"><span class="pln">  assert</span><span class="pun">(*</span><span class="pln">min_log2_tile_cols </span><span class="pun">&lt;=</span><span class="pln"> </span><span class="pun">*</span><span class="pln">max_log2_tile_cols</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="59" onclick="window.location.hash='#59'"></td><td class="FileContents-lineContents" id="59"><span class="pun">}</span></td></tr></table></div> <!-- Container --></div> <!-- Site-content --><footer class="Site-footer"><div class="Footer"><span class="Footer-poweredBy">Powered by <a href="https://gerrit.googlesource.com/gitiles/">Gitiles</a>| <a href="https://policies.google.com/privacy">Privacy</a></span><span class="Footer-formats"><a class="u-monospace Footer-formatsItem" href="?format=TEXT">txt</a> <a class="u-monospace Footer-formatsItem" href="?format=JSON">json</a></span></div></footer></body></html>