---
layout: default
title: Download
src_c:
    - libsrs2-1.0.18.tar.gz
    - libsrs2-1.0.17.tar.gz
    - libsrs2-1.0.16.tar.gz
src_perl:
    - Mail-SRS-0.31.tar.gz
    - Mail-SRS-0.30.tar.gz
    - Mail-SRS-0.29.tar.gz
---

<h1>C Implementation (libsrs2 source)</h1>

This is the core libsrs2 against which you should be linking your MTA.
Please submit patches via <a href="https://github.com/shevek/libsrs2/">github</a>.

<ul>
{% for _ in page.src_c %}
<li><a href="srs/{{_}}">{{_}}</a></li>
{% endfor %}
</ul>

<h1>Developers</h1>

Check the code out from github:

<ul>
<li><a href="https://github.com/shevek/libsrs2/">https://github.com/shevek/libsrs2/</a></li>
</ul>

{% comment %}
<h1>C Implementation (libsrs2 binaries)</h1>

	<p>
If your distribution isn't listed, or the package is out of date,
please contribute a new package. The developers might not have access
to a system running your distribution.
	</p>
{% endcomment %}

<h1>Perl Implementation (Mail::SRS)</h1>

The Perl implementation is also available from
<a href="https://metacpan.org/pod/Mail::SRS">CPAN</a>. This is the
reference implementation and any other implementation MUST return
the same results as this Perl implementation.

<ul>
{% for _ in page.src_perl %}
<li><a href="srs/{{_}}">{{_}}</a></li>
{% endfor %}
</ul>
