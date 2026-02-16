---
layout: default
title: Home
redirect_from:
  - /srs/
---

<h1>Welcome</h1>

<b>libsrs2 is the next generation SRS library from the original
designer of SRS.</b> It implements the Sender Rewriting Scheme, a
part of the <a href="https://www.libspf2.net/">SPF</a>/SRS protocol
pair. Libsrs2 has been written from an entirely clean codebase with
<b>compliance</b>, <b>speed</b> and <b>versatility</b> in mind. It is
<b>platform independent</b> and has <b>no external dependencies</b>.
It is <b>thread-safe</b> and <b>heap-safe</b>,
and is suitable for <b>large scale applications</b> and
<b>embedded systems</b> and <b>can operate without many standard
system facilities</b>.

<ul>
<li><a href="srs/srs.pdf">I want to find out about SRS.</a> (technical paper)</li>
{% comment %}
<li><a href="docs/mta-users.html">I want to configure my MTA for SRS.</a> (MTA users' guide)</li>
<li><a href="docs/mta-patches.html">I want to patch an MTA for SRS.</a> (MTA patchers' guide)</li>
{% endcomment %}
<li><a href="download.html">I want to download libsrs2.</a> (download page)</li>
<li><a href="docs/index.html">I want to read documentation for libsrs2.</a> (documentation page)</li>
<li><a href="support.html">I want support for SRS or libsrs2.</a> (support page)</li>
<li><a href="support.html">I found a bug!</a> (support page)</li>
</ul>

<h1>What is SPF?</h1>

SPF is one of a family of systems which aim to prevent email
address forgery. A good SPF implementation is available from <a
href="https://www.libspf2.net/">https://www.libspf2.net/</a>.

The present
SMTP standard for email allows anyone to forge anyone else's email
address. This means I could send anyone a message claiming to be from
you, and only an email expert would be able to tell the difference.
SPF allows your mail servers to easily distinguish forgeries from real
mail. Importantly, SPF works before the message body is transmitted,
saving you the bandwidth cost of downloading the message and the CPU
cost of filtering it.

<h1>What is SRS?</h1>

SPF (and related systems) present a challenge to forwarders, since
the envelope sender address might be seen by the destination as a
forgery by the forwarding host. Forwarding services must rewrite the
envelope sender address, while encapsulating the original sender and
preventing relay attacks by spammers.

The Sender Rewriting Scheme, or SRS, provides a standard for
this rewriting which makes forwarding compatible with these
address verification schemes, preserves bounce functionality
and is not vulnerable to attacks by spammers. A graphical
explanation is available <a href="overview.html">here</a> 
and a technical
document describing the protocol and its strengths is at <a
href="srs/srs.pdf">here</a>.

<h1>Why SRS?</h1>

<dl>
<dt><b>It works with all address verification schemes..</b></dt>
<dd>SRS is an exceptionally clean and simple design which brings
	compatibility not just with SPF, but with many other ad-hoc
	address verification schemes, some of which are used by
	major ISPs.</dd>
<dt><b>It is well defined and secure.</b></dt>
<dd>The <a href="srs/srs.pdf">paper on SRS</a> describes in detail
	how the protocol works and why it is invulnerable to attack.</dd>
<dt><b>SRS does not require changes to protocol or semantics.</b></dt>
<dd>SRS exists as a rewriting system entirely within current SMTP
	practices. Many alternative systems require changes to either SMTP
	syntax, semantics or both. SRS requires neither.</dd>
<dt><b>It has minimal impact.</b></dt>
<dd>While anyone can implement SRS, only forwarders <i>need</i>
	to implement SRS. No modifications are required by any other
	party to MSAs, MTAs, MUAs or other software.</dd>
</dl>

{% comment %}
<h1>How to Contribute</h1>

Contact <a href="/support.html">the developers</a>. Can you (for example)
<ul>
<li><b>Write or maintain patches against MTAs?</b>
	<ul>
	<li>courier</li>
	<li>zmailer</li>
	<li>qmail</li>
	</ul>
</li>
<li><b>Take responsibility for a distribution?</b>
	<ul>
	<li>Debian (Maintainer needed)</li>
	<li>Red Hat and Mandrake (Packager needed)</li>
	<li>Slackware</li>
	<li>FreeBSD ports (Submitter needed)</li>
	<li>OS/X fink</li>
%#	<li>any others?</li>
	</ul>
</li>
<li><b>Write bindings for a language?</b>
	<ul>
	<li>Python</li>
%#	<li>any others?</li>
	</ul>
</li>
</ul>
{% endcomment %}
