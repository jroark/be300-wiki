function opencomment(url,w,h) {
	if (w=="") 	{ w=520; }
	if (h=="") 	{ h=520; }
	window.open(url,'comment','status=yes, scrollbars=yes,resizable=yes,width='+w+',height='+h);
}

function popup(t, w, h) {
	window.open('pivot/extra/photo.php?img='+t+'&w='+w+'&h='+h+'&t='+t,'photo','width='+w+',height='+h);  
}
