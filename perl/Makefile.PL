require 5.005002;
use strict;
use ExtUtils::MakeMaker;

my $cflags = q[-DHAVE_CONFIG_H];
$cflags .= q[ -Wall -g -O0] if -d '/home/shevek';	# Maintainer/gcc stuff

WriteMakefile(
	NAME			=> 'Mail::SRS_XS',
	VERSION_FROM	=> 'SRS_XS.pm',
    ABSTRACT_FROM	=> "SRS_XS.pm",
	AUTHOR			=> 'Shevek <cpan@anarres.org>',
	PREREQ_PM		=> {
					'Mail::SRS'		=> 0.30,
						},
	OPTIMIZE		=> $cflags,
	LIBS			=> [ '-L../libsrs2/.libs -lsrs2', ],
);

sub MY::postamble {
	my $self = shift;
	my $old = $self->MM::postamble(@_);
	chomp($old);
	my $new = <<'EON';

$(INST_DYNAMIC) : ../libsrs2/.libs/libsrs2.so.*

.PHONY : aux dep readme

CPP.c : typemap

aux : readme dep

readme : CPP.pm
	perldoc -t CPP.pm > README

dep : $(C_FILES) $(H_FILES)
	$(RM_F) .depend
	$(CC) -MM $(INC) -isystem $(PERL_INC) $(CCFLAGS) $(C_FILES) >> .depend

EON
	return $old . $new;
}
