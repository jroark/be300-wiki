<!doctype html public "-//W3C//DTD HTML 4.0 Transitional//EN"
 "http://www.w3.org/TR/REC-html40/loose.dtd">
<html>
<title>CVS log for ce/dumpnkbin/dumpnkbin.c</title>
<!-- hennerik CVSweb $Revision: 1.112 $ -->
</head>
<body text="#000000" bgcolor="#ffffff">
<img src="/icons/apache_pb.gif"> <h1 align="center">CVS log for ce/dumpnkbin/dumpnkbin.c</h1>
<A HREF="/cgi-bin/cvsweb.cgi/ce/dumpnkbin/#dumpnkbin.c"><IMG SRC="/icons/back.gif" ALT="[BACK]" BORDER="0" WIDTH="20" HEIGHT="22"></A>
 <b>Up to  <a href="/cgi-bin/cvsweb.cgi/#dirlist">[Linux4.BE]</a> / <a href="/cgi-bin/cvsweb.cgi/ce/#dirlist">ce</a> / <a href="/cgi-bin/cvsweb.cgi/ce/dumpnkbin/#dirlist">dumpnkbin</a></b><p>
<A HREF="#diff">Request diff between arbitrary revisions</A>
<HR NOSHADE>
Default branch: 1<BR>
<HR size=1 NOSHADE><a NAME="rev1.1"></a><a NAME="HEAD"></a>
 Revision <A HREF="dumpnkbin.c?rev=1.1&amp;content-type=text/x-cvsweb-markup"><b>1.1</b></A> / (<A HREF="/cgi-bin/cvsweb.cgi/~checkout~/ce/dumpnkbin/dumpnkbin.c?rev=1.1&amp;content-type=text/plain" target="cvs_checkout" onClick="window.open('/cgi-bin/cvsweb.cgi/~checkout~/ce/dumpnkbin/dumpnkbin.c?rev=1.1&amp;content-type=text/plain','cvs_checkout','resizeable,scrollbars');"><b>download</b></A>) - <a href="/cgi-bin/cvsweb.cgi/ce/dumpnkbin/dumpnkbin.c?annotate=1.1">annotate</a> - <A HREF="/cgi-bin/cvsweb.cgi/ce/dumpnkbin/dumpnkbin.c?r1=1.1">[select for diffs]</A>
, <i>Mon Sep 13 20:54:17 2004 UTC</i> (5 months, 1 week ago) by <i>filip</i>
<BR>CVS Tags: <b><A HREF="dumpnkbin.c?only_with_tag=HEAD">HEAD</A>
</b><BR>Diff<PRE>
From Holger Schurig: Here is the file that dumps some info from an Windows CE nk.bin, maybe it can help others when they want to port a free Operating System to some platform.
</PRE>
<HR NOSHADE><A NAME=diff>
This form allows you to request diff's between any two
revisions of a file.  You may select a symbolic revision
name using the selection box or you may type in a numeric
name using the type-in text box.
</A><P>
<FORM METHOD="GET" ACTION="/cgi-bin/cvsweb.cgi/ce/dumpnkbin/dumpnkbin.c.diff" NAME="diff_select">
Diffs between 
<SELECT NAME="r1">
<OPTION VALUE="text" SELECTED>Use Text Field
<OPTION VALUE="0.1:MAIN">MAIN
<OPTION VALUE="1.1:HEAD">HEAD
</SELECT>
<INPUT TYPE="TEXT" SIZE="12" NAME="tr1" VALUE="1.1" onChange='document.diff_select.r1.selectedIndex=0'>
 and 
<SELECT NAME="r2">
<OPTION VALUE="text" SELECTED>Use Text Field
<OPTION VALUE="0.1:MAIN">MAIN
<OPTION VALUE="1.1:HEAD">HEAD
</SELECT>
<INPUT TYPE="TEXT" SIZE="12" NAME="tr2" VALUE="1.1" onChange='document.diff_select.r2.selectedIndex=0'>
<BR>Type of Diff should be a&nbsp;<SELECT NAME="f">
<OPTION VALUE=h SELECTED>Colored Diff<OPTION VALUE=H>Long Colored Diff<OPTION VALUE=u>Unidiff<OPTION VALUE=c>Context Diff</SELECT><INPUT TYPE=SUBMIT VALUE="  Get Diffs  ">
</FORM>
<HR noshade>
<A name=logsort></A>
<FORM METHOD="GET" ACTION="/cgi-bin/cvsweb.cgi/ce/dumpnkbin/dumpnkbin.c">
Sort log by: 
<SELECT NAME="logsort" onchange="submit()">
<OPTION VALUE=cvs>Not sorted<OPTION VALUE=date SELECTED>Commit date<OPTION VALUE=rev>Revision</SELECT>
<INPUT TYPE=SUBMIT VALUE="  Sort  ">
</FORM>
<hr noshade><address><font size=-1>CVSweb by &lt;zeller@think.de&gt;</font></address></body></html>
</BODY></HTML>
