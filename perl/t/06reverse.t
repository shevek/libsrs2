use strict;
use warnings;
use blib;

use Test::More tests => 4;

BEGIN { use_ok('Mail::SRS_XS'); }

my $srs = new Mail::SRS_XS({
				Secret	=> "foo",
					});
ok(defined $srs);
ok(UNIVERSAL::isa($srs, 'Mail::SRS_XS'));

my $addr = 'usera@hosta.com';
my $new = $srs->forward($addr, 'userb@hostb.com');
my $old = $srs->reverse($new);
is($old, $addr, 'Reversal works');
