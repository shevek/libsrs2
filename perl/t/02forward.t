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

my $new = $srs->forward('usera@hosta.com', 'userb@hostb.com');
like($new, qr/^SRS/, 'Forwarding works');
