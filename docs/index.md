---
layout: default
title: Documentation
redirect_from:
  - /docs/api.html
---

<h1>Papers describing SRS (for all interested parties)</h1>

Shevek's paper describing SRS is available in several formats. This
paper documents the design of the protocol, the possible attacks
against the protocol and why the design is invulnerable to these
attacks. Any modification of the protocol should be required to undergo
the same form of analysis. Modify the protocol at your own risk.

<ul>
	<li><a href="/srs/srs.pdf">The SRS paper</a></li>
</ul>

{% comment %}
<h1>Documentation for libsrs2 Users</h1>

See the <a href="api.html">API documentation page</a>.

<h1>Documentation for MTA Coders</h1>

See the <a href="mta-patches.html">MTA patches page</a>.

<h1>Documentation for MTA Users</h1>

See the <a href="mta-users.html">MTA users page</a>.

<h1>Documentation for Distributors</h1>

<ul>
<li><a href="http://fink.sourceforge.net/help/index.php"><b>fink</b> - notes on submitting</a></li>
<li><a href="http://www.debian.org/devel/"><b>debian</b> - notes on submitting</a></li>
</ul>
{% endcomment %}

<h1>Headers</h1>

<code>
#include &lt;srs2.h&gt;
</code>

The srs2 header file must be included.

<h1>Types</h1>

<code>
srs_t
</code>

The <tt>srs_t</tt> structure is a handle to an SRS rewriting
engine. There are two ways to construct a handle: <tt>srs_new()</tt> and
<tt>srs_init()</tt>. It is recommended that <tt>srs_new()</tt> be used
unless you are scared of malloc().

<h1>Functions</h1>

The library does not modify any input arguments allocated in the
memory of the caller, except possibly for the srs handle. Input email
addresses are never modified.

<code>
srs_t *srs_new();
</code>

Returns a new handle for accessing the SRS rewriting engine. This is
the first function you will call in any SRS sequence. If you call
<tt>srs_new()</tt>, you do <b>not</b> need to call <tt>srs_init()</tt>.

<code>
void srs_free(srs_t *srs);
</code>

Destroys a handle previously allocated by <tt>srs_new()</tt>. The
memory previously used to store secrets is overwritten using
<tt>memset()</tt> and deallocated using <tt>free()</tt>.

<code>
void srs_add_secret(srs_t *srs, const char *secret);
</code>

Adds a new secret to the SRS library. Only the first secret is
used for forward-rewriting addresses. All secrets are attempted for
reverse-rewriting addresses. You must call this function at least once.
Storage for secrets is allocated using <tt>strdup()</tt>. The memory
containing the original copy of the secret may be freed or reused
after this call.

<code>
int srs_forward(srs_t *srs, char *buf, int buflen,
				const char *sender, const char *alias);
</code>

Rewrites <tt>sender</tt> to appear from the domain given in
<tt>alias</tt>. <tt>alias</tt> may be an email address or a domain.
	The NUL-terminated rewritten address is placed in <tt>buf</tt>,
	which is of size <tt>buflen</tt>. If <tt>buflen</tt> is too
	small, <tt>SRS_EBUFTOOSMALL</tt> is returned.
A conservative assumption is that the rewritten address will be of
size at most <tt>strlen(sender) + strlen(alias) + 64</tt>.

<code>
int srs_forward_alloc(srs_t *srs, char **sptr,
				const char *sender, const char *alias);
</code>

Similar to <tt>srs_forward()</tt> except that libsrs2 is responsible
for allocating memory to contain the parsed address. A pointer
to the rewritten address is written into *sptr. The caller is
responsible for freeing this memory. On error, an error code is
returned and NULL is written into *sptr. This function never returns
<tt>SRS_EBUFTOOSMALL</tt>.

<code>
int srs_reverse(srs_t *srs, char *buf, int buflen, const char *sender);
</code>

Rewrites a previously rewritten SRS address into its original form.
	The NUL-terminated rewritten address is placed in <tt>buf</tt>,
	which is of size <tt>buflen</tt>. If <tt>buflen</tt> is too
	small, <tt>SRS_EBUFTOOSMALL</tt> is returned.
The rewritten address will be no longer than <tt>sender</tt>.

<code>
int srs_reverse_alloc(srs_t *srs, char **sptr, const char *sender);
</code>

Similar to <tt>srs_reverse()</tt> except that libsrs2 is responsible
for allocating memory to contain the parsed address. A pointer
to the rewritten address is written into *sptr. The caller is
responsible for freeing this memory. On error, an error code is
returned and NULL is written into *sptr. This function never returns
<tt>SRS_EBUFTOOSMALL</tt>.

<code>
const char * srs_strerror(int code);
</code>

Translates a return code from libsrs2 into a textual description of the
error. See <b>Return codes</b> below.

<code>
srs_t srs;<br />
srs_init(srs_t *srs);
</code>

An alternative way to construct a rewriting engine. This option
is not recommended unless you do not have <tt>malloc()</tt> since
it requires recompiling the application if the size of the srs_t
structure changes. Trust me, you probably have <tt>malloc()</tt>.

Users calling <tt>srs_init()</tt> are responsible for updating
<tt>srs->secrets</tt> and <tt>srs->numsecrets</tt>. This is inelegant
and discouraged. It may improve in the future.

<code>
int srs_set_separator(srs_t *srs, char c); <br />
char srs_get_separator(srs_t *srs);
</code>

Get or set the initial separator used in rewriting. The initial
separator is the separator appearing immediately after the SRS0 or SRS1
tag. Administrators may wish to set this separator to some character
which their MTA recognises as an email address delimiter, so that
messages incoming to SRS addresses may be recognised as all those
directed to an <tt>srs0</tt> or <tt>srs1</tt> account. Permissible
characters are <tt>+</tt>, <tt>-</tt> and <tt>=</tt>. Returns
<tt>SRS_ESEPARATORINVALID</tt> if an invalid separator is given.

<code>
int srs_set_noforward(srs_t *srs, srs_bool b); <br />
char srs_get_noforward(srs_t *srs);
</code>

Set or get the flag which disables all SRS forwards rewriting.

<code>
int srs_set_noreverse(srs_t *srs, srs_bool b); <br />
char srs_get_noreverse(srs_t *srs);
</code>

Set or get the flag which disables all SRS reverse rewriting.

<h1>Return codes</h1>

SRS functions return codes indicating the status of the requested
operation. Return codes fall into classes, so that the user may
treat all return codes in a class identically, for the purpose of
client program behaviour. The class of a return code is given by
the <tt>SRS_ERROR_TYPE(x)</tt> macro. Currently defined classes of
return code are:

<ul>
<li><b><tt>SRS_ERRTYPE_NONE</tt></b>: Unclassified errors.</li>
<li><b><tt>SRS_ERRTYPE_CONFIG</tt></b>: The SRS library is misconfigured or a misconfiguration was attempted.</li>
<li><b><tt>SRS_ERRTYPE_INPUT</tt></b>: The input from the client program was invalid.</li>
<li><b><tt>SRS_ERRTYPE_SYNTAX</tt></b>: An SRS address was syntactically invalid.</li>
<li><b><tt>SRS_ERRTYPE_SRS</tt></b>: An SRS address was syntactically valid, but semantically invalid.</li>
</ul>

The specific return codes from SRS functions are
mostly self-explanatory. They are listed here. See also
<tt>srs_strerror()</tt>, which returns a textual error message for
any return code.

<dl>

<dt><b>Errors of type <tt>SRS_ERRTYPE_NONE</tt>:</b></dt>
<dt><tt>SRS_SUCCESS</tt></dt>
<dd>The operation was successful.</dd>
<dt><tt>SRS_ENOTSRSADDRESS</tt></dt>
<dd>The address given cannot be reversed, since it is not an SRS address.</dd>
<dt><tt>SRS_ENOTREWRITTEN</tt></dt>
<dd>The address was not rewritten, probably because the configuration
denies forwards or reverse rewriting.</dd>

<dt><b>Errors of type <tt>SRS_ERRTYPE_CONFIG</tt>:</b></dt>
<dt><tt>SRS_ENOSECRETS</tt></dt>
<dd>Cannot create or parse an SRS address without any secrets.</dd>
<dt><tt>SRS_ESEPARATORINVALID</tt></dt>
<dd>The SRS separator was invalid.</dd>

<dt><b>Errors of type <tt>SRS_ERRTYPE_INPUT</tt>:</b></dt>
<dt><tt>SRS_ENOSENDERATSIGN</tt></dt>
<dd>The sender address did not contain an at sign (<tt>@</tt>).</dd>
<dt><tt>SRS_EBUFTOOSMALL</tt></dt>
<dd>The buffer given was too small to contain the result.</dd>

<dt><b>Errors of type <tt>SRS_ERRTYPE_SYNTAX</tt>:</b></dt>
<dt><tt>SRS_ENOSRS0HOST</tt></dt>
<dd>Could not find the original hostname in the SRS0 local-part.</dd>
<dt><tt>SRS_ENOSRS0USER</tt></dt>
<dd>Could not find the original username in the SRS0 local-part.</dd>
<dt><tt>SRS_ENOSRS0HASH</tt></dt>
<dd>Could not find the hash in the SRS0 local-part.</dd>
<dt><tt>SRS_ENOSRS0STAMP</tt></dt>
<dd>Could not find the timestamp in the SRS0 local-part.</dd>
<dt><tt>SRS_ENOSRS1HOST</tt></dt>
<dd>Could not find the original hostname in the SRS1 local-part.</dd>
<dt><tt>SRS_ENOSRS1USER</tt></dt>
<dd>Could not find the original username in the SRS1 local-part.</dd>
<dt><tt>SRS_ENOSRS1HASH</tt></dt>
<dd>Could not find the hash in the SRS1 local-part.</dd>
<dt><tt>SRS_EBADTIMESTAMPCHAR</tt></dt>
<dd>A bad (non base-32) character appeared in the SRS timestamp.</dd>
<dt><tt>SRS_EHASHTOOSHORT</tt></dt>
<dd>Hash in SRS address is too short (&lt; hashmin, insufficient protection).</dd>

<dt><b>Errors of type <tt>SRS_ERRTYPE_SRS</tt></b></dt>
<dt><tt>SRS_ETIMESTAMPOUTOFDATE</tt></dt>
<dd>The SRS timestamp was out of date.</dd>
<dt><tt>SRS_EHASHINVALID</tt></dt>
<dd>Hash in SRS address is invalid.</dd>
</dl>

<h1>Testing</h1>

The source distribution contains a subdirectory <tt>perl/</tt>,
which contains a small test suite for comparing the behaviour of
libsrs2 with the behaviour of the reference implementation Mail::SRS.
To run the test suite, build libsrs2, then
from the <tt>perl/</tt> subdirectory, type:

<code>
perl Makefile.PL <br />
make test <br />
</code>

libsrs2 is a compliant library and may be used as a "gold standard"
for testing other implementations. We aim to provide bindings for
various languages to make this easier. If you write bindings for
libsrs2 to a new language, please send them to shevek [ta] anarres.org
for inclusion in the project.
