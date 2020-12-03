<!DOCTYPE html><html lang="en"><head><meta charset="utf-8"><title>libvpx/vp8/common/rtcd.c - platform/external/libvpx - Git at Google</title><link rel="stylesheet" type="text/css" href="/+static/base.YxgkRP5in5Amk_r7PKOxxA.cache.css"><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.AOMOBqJIPcDq491E2ExAAw.cache.css"><!-- default customHeadTagPart --></head><body class="Site"><header class="Site-header"><div class="Header"><a class="Header-image" href="/"><img src="//www.gstatic.com/images/branding/lockups/2x/lockup_git_color_108x24dp.png" width="108" height="24" alt="Google Git"></a><div class="Header-menu"> <a class="Header-menuItem" href="https://accounts.google.com/AccountChooser?service=gerritcodereview&amp;continue=https://android.googlesource.com/login/platform/external/libvpx/%2B/e8544063f08d093e211247d09d74e5bf86976dd5/libvpx/vp8/common/rtcd.c">Sign in</a> </div></div></header><div class="Site-content"><div class="Container "><div class="Breadcrumbs"><a class="Breadcrumbs-crumb" href="/?format=HTML">android</a> / <a class="Breadcrumbs-crumb" href="/platform/">platform</a> / <a class="Breadcrumbs-crumb" href="/platform/external/">external</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/">libvpx</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/e8544063f08d093e211247d09d74e5bf86976dd5">e8544063f08d093e211247d09d74e5bf86976dd5</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/e8544063f08d093e211247d09d74e5bf86976dd5/">.</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/e8544063f08d093e211247d09d74e5bf86976dd5/libvpx">libvpx</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/e8544063f08d093e211247d09d74e5bf86976dd5/libvpx/vp8">vp8</a> / <a class="Breadcrumbs-crumb" href="/platform/external/libvpx/+/e8544063f08d093e211247d09d74e5bf86976dd5/libvpx/vp8/common">common</a> / <span class="Breadcrumbs-crumb">rtcd.c</span></div><div class="u-sha1 u-monospace BlobSha1">blob: 0b371b094aa72ed9ad5e021f57a8e1ec2e7b55a1 [<a href="/platform/external/libvpx/+/e8544063f08d093e211247d09d74e5bf86976dd5/libvpx/vp8/common/rtcd.c">file</a>] [<a href="/platform/external/libvpx/+log/e8544063f08d093e211247d09d74e5bf86976dd5/libvpx/vp8/common/rtcd.c">log</a>] [<a href="/platform/external/libvpx/+blame/e8544063f08d093e211247d09d74e5bf86976dd5/libvpx/vp8/common/rtcd.c">blame</a>]</div><table class="FileContents"><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="1" onclick="window.location.hash='#1'"></td><td class="FileContents-lineContents" id="1"><span class="com">/*</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="2" onclick="window.location.hash='#2'"></td><td class="FileContents-lineContents" id="2"><span class="com"> *  Copyright (c) 2011 The WebM project authors. All Rights Reserved.</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="3" onclick="window.location.hash='#3'"></td><td class="FileContents-lineContents" id="3"><span class="com"> *</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="4" onclick="window.location.hash='#4'"></td><td class="FileContents-lineContents" id="4"><span class="com"> *  Use of this source code is governed by a BSD-style license</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="5" onclick="window.location.hash='#5'"></td><td class="FileContents-lineContents" id="5"><span class="com"> *  that can be found in the LICENSE file in the root of the source</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="6" onclick="window.location.hash='#6'"></td><td class="FileContents-lineContents" id="6"><span class="com"> *  tree. An additional intellectual property rights grant can be found</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="7" onclick="window.location.hash='#7'"></td><td class="FileContents-lineContents" id="7"><span class="com"> *  in the file PATENTS.  All contributing project authors may</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="8" onclick="window.location.hash='#8'"></td><td class="FileContents-lineContents" id="8"><span class="com"> *  be found in the AUTHORS file in the root of the source tree.</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="9" onclick="window.location.hash='#9'"></td><td class="FileContents-lineContents" id="9"><span class="com"> */</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="10" onclick="window.location.hash='#10'"></td><td class="FileContents-lineContents" id="10"><span class="com">#include</span><span class="pln"> </span><span class="str">&quot;vpx_config.h&quot;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="11" onclick="window.location.hash='#11'"></td><td class="FileContents-lineContents" id="11"><span class="com">#define</span><span class="pln"> RTCD_C</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="12" onclick="window.location.hash='#12'"></td><td class="FileContents-lineContents" id="12"><span class="com">#include</span><span class="pln"> </span><span class="str">&quot;vp8_rtcd.h&quot;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="13" onclick="window.location.hash='#13'"></td><td class="FileContents-lineContents" id="13"><span class="com">#include</span><span class="pln"> </span><span class="str">&quot;vpx_ports/vpx_once.h&quot;</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="14" onclick="window.location.hash='#14'"></td><td class="FileContents-lineContents" id="14"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="15" onclick="window.location.hash='#15'"></td><td class="FileContents-lineContents" id="15"><span class="kwd">extern</span><span class="pln"> </span><span class="kwd">void</span><span class="pln"> vpx_scale_rtcd</span><span class="pun">(</span><span class="kwd">void</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="16" onclick="window.location.hash='#16'"></td><td class="FileContents-lineContents" id="16"></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="17" onclick="window.location.hash='#17'"></td><td class="FileContents-lineContents" id="17"><span class="kwd">void</span><span class="pln"> vp8_rtcd</span><span class="pun">()</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="18" onclick="window.location.hash='#18'"></td><td class="FileContents-lineContents" id="18"><span class="pun">{</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="19" onclick="window.location.hash='#19'"></td><td class="FileContents-lineContents" id="19"><span class="pln">    vpx_scale_rtcd</span><span class="pun">();</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="20" onclick="window.location.hash='#20'"></td><td class="FileContents-lineContents" id="20"><span class="pln">    once</span><span class="pun">(</span><span class="pln">setup_rtcd_internal</span><span class="pun">);</span></td></tr><tr class="u-pre u-monospace FileContents-line"><td class="u-lineNum u-noSelect FileContents-lineNum" data-line-number="21" onclick="window.location.hash='#21'"></td><td class="FileContents-lineContents" id="21"><span class="pun">}</span></td></tr></table></div> <!-- Container --></div> <!-- Site-content --><footer class="Site-footer"><div class="Footer"><span class="Footer-poweredBy">Powered by <a href="https://gerrit.googlesource.com/gitiles/">Gitiles</a>| <a href="https://policies.google.com/privacy">Privacy</a></span><span class="Footer-formats"><a class="u-monospace Footer-formatsItem" href="?format=TEXT">txt</a> <a class="u-monospace Footer-formatsItem" href="?format=JSON">json</a></span></div></footer></body></html>