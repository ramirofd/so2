#!/usr/bin/perl
#
# file_upload.pl - Demonstration script for file uploads
# over HTML form.
#
# This script should function as is.  Copy the file into
# a CGI directory, set the execute permissions, and point
# your browser to it. Then modify if to do something
# useful.
#
# Author: Kyle Dent
# Date: 3/15/01
#

use CGI;
use strict;
use IPC::Open3;
use Net::Address::IP::Local;

my $PROGNAME = "file_upload.pl";

my $cgi = new CGI();

my $modname = $cgi->param('modname');

my $address = Net::Address::IP::Local->public;

use strict 'refs';

local (*BO, *BI);
my $pid = open3(\*BI, \*BO, \*BO, "/var/www/cgi-bin/wrmmod $modname");

print  $cgi->redirect("http://${address}/app/modules.html");