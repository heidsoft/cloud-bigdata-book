#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# $Id: status.cgi,v 1.6.2.1 2004/10/25 17:39:58 gespinasse Exp $
#


print "<br>";
print <<END
<tr>   
<td><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td><img style="margin-top:30px;" src="sbztbg.jpg" width="145" height="30" /></td>
<td align="right" valign="bottom">&nbsp;</td>
</tr>
</table></td>
</tr>    
END
;

print "<tr><td><table width='100%' border='1' cellpadding='0' cellspacing='0' bordercolor='#d0d0d0' class='table' style='border-collapse:collapse; margin-top:3px; margin-bottom:3px; text-align:center;'>\n";
open(DF,'/bin/df -m |');
while(<DF>)
{
	if ($_ =~ m/^Filesystem/ )
	{
		print <<END
<tr>
<td width="140" height="32" bgcolor="5489c0" class="bai">设备</td>
<td width="140" bgcolor="5489c0" class="bai">目录</td>
<td width="140" bgcolor="5489c0" class="bai">大小</td>
<td width="140" bgcolor="5489c0" class="bai">利用</td>
<td width="140" bgcolor="5489c0" class="bai">空闲</td>
<td width="258" bgcolor="5489c0" class="bai" colspan="2">百分比</td>
</tr>
END
;
	}
	else
	{
		my ($device,$size,$used,$free,$percent,$mount) = split;
		if($mount eq "/" || $mount eq "/app"){
		print <<END
<tr>
<td>$device</td>
<td>$mount</td>
<td>$size M</td>
<td>$used M</td>
<td>$free M</td>
<td width="170" align="right">
END
;
		&percentbar($percent);
		print <<END
</td><td width="48" >$percent</td>
</td>
</tr>
END
;
}
	}
}
close DF;
print "</table></td></tr></table></td></tr>\n";


sub getpid($) {
    my $pidfile = shift;

    if (! open(FILE, "${pidfile}")) {
	return 0;
    }
    my $pid = <FILE>;
    chomp $pid;
    close FILE;
    return $pid;
}

sub getprocname($) {
    my $pid = shift;
    if ($pid == 0) {
	return '';
    }
    if (! open(FILE, "/proc/${pid}/status")) {
	return '';
    }

    my $cmd = '';
    while (<FILE>) {
	if (/^Name:\W+(.*)/) {
	    $cmd = $1;
	    last;
	}
    }
    close FILE;
    return $cmd;
}


sub getcmdline($) {
    my $pid = shift;
    if ($pid == 0) {
	return '';
    }
    if (! open(FILE, "/proc/${pid}/cmdline")) {
	return '';
    }
    my $cmdline = <FILE>;
    close FILE;

    return $cmdline;
}


sub percentbar
{
  my $percent = $_[0];
  my $fg = '#196CBC';
  my $bg = '#e2e2e2';

  if ($percent =~ m/^(\d+)%$/ )
  {
    print <<END
<table width='160' border='1' cellspacing='0' cellpadding='0' >
<tr>
END
;
    if ($percent eq "100%") {
      print "<td width='100%' height='10' bgcolor='$fg' style='background-color:$fg;border-style:solid;border-width:1px;border-color:$bg'>"
    } elsif ($percent eq "0%") {
      print "<td width='100%' height='10' bgcolor='$bg' style='background-color:$bg;border-style:solid;border-width:1px;border-color:$bg'>"
    } else {
      print "<td width='$percent' height='10' bgcolor='$fg' style='background-color:$fg;border-style:solid;border-width:1px;border-color:$bg'></td><td width='" . (100-$1) . "%' bgcolor='$bg' style='background-color:$bg;border-style:solid;border-width:1px;border-color:$bg'>"
    }
    print <<END
</td></tr></table>
END
;
  }
}
