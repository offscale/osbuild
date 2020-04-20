#ifndef OSBUILD_LINUX_VERSION_H
#define OSBUILD_LINUX_VERSION_H

#if __linux__
/* Split a stringbuf into a key/value pair.
   Return the key, leaving the stripped value in the stringbuf. */
static const char *
stringbuf_split_key(svn_stringbuf_t *buffer, char delim)
{
  char *key;
  char *end;

  end = strchr(buffer->data, delim);
  if (!end)
    return NULL;

  svn_stringbuf_strip_whitespace(buffer);

  /* Now we split the currently allocated buffer in two parts:
      - a const char * HEAD
      - the remaining stringbuf_t. */

  /* Create HEAD as '\0' terminated const char * */
  key = buffer->data;
  end = strchr(key, delim);
  *end = '\0';

  /* And update the TAIL to be a smaller, but still valid stringbuf */
  buffer->data = end + 1;
  buffer->len -= 1 + end - key;
  buffer->blocksize -= 1 + end - key;

  svn_stringbuf_strip_whitespace(buffer);

  return key;
}

/* Parse `/usr/bin/lsb_rlease --all` */
static const char *
lsb_release(apr_pool_t *pool)
{
  static const char *const args[3] =
    {
      "/usr/bin/lsb_release",
      "--all",
      NULL
    };

  const char *distributor = NULL;
  const char *description = NULL;
  const char *release = NULL;
  const char *codename = NULL;

  apr_proc_t lsbproc;
  svn_stream_t *lsbinfo;
  svn_error_t *err;

  /* Run /usr/bin/lsb_release --all < /dev/null 2>/dev/null */
  {
    apr_file_t *stdin_handle;
    apr_file_t *stdout_handle;

    err = svn_io_file_open(&stdin_handle, SVN_NULL_DEVICE_NAME,
                           APR_READ, APR_OS_DEFAULT, pool);
    if (!err)
      err = svn_io_file_open(&stdout_handle, SVN_NULL_DEVICE_NAME,
                             APR_WRITE, APR_OS_DEFAULT, pool);
    if (!err)
      err = svn_io_start_cmd3(&lsbproc, NULL, args[0], args, NULL, FALSE,
                              FALSE, stdin_handle,
                              TRUE, NULL,
                              FALSE, stdout_handle,
                              pool);
    if (err)
      {
        svn_error_clear(err);
        return NULL;
      }
  }

  /* Parse the output and try to populate the  */
  lsbinfo = svn_stream_from_aprfile2(lsbproc.out, TRUE, pool);
  if (lsbinfo)
    {
      for (;;)
        {
          svn_boolean_t eof = FALSE;
          svn_stringbuf_t *line;
          const char *key;

          err = svn_stream_readline(lsbinfo, &line, "\n", &eof, pool);
          if (err || eof)
            break;

          key = stringbuf_split_key(line, ':');
          if (!key)
            continue;

          if (0 == svn_cstring_casecmp(key, "Distributor ID"))
            distributor = line->data;
          else if (0 == svn_cstring_casecmp(key, "Description"))
            description = line->data;
          else if (0 == svn_cstring_casecmp(key, "Release"))
            release = line->data;
          else if (0 == svn_cstring_casecmp(key, "Codename"))
            codename = line->data;
        }
      err = svn_error_compose_create(err,
                                     svn_stream_close(lsbinfo));
      if (err)
        {
          svn_error_clear(err);
          apr_proc_kill(&lsbproc, SIGKILL);
          return NULL;
        }
    }

  /* Reap the child process */
  err = svn_io_wait_for_cmd(&lsbproc, "", NULL, NULL, pool);
  if (err)
    {
      svn_error_clear(err);
      return NULL;
    }

  if (description)
    return apr_psprintf(pool, "%s%s%s%s", description,
                        (codename ? " (" : ""),
                        (codename ? codename : ""),
                        (codename ? ")" : ""));
  if (distributor)
    return apr_psprintf(pool, "%s%s%s%s%s%s", distributor,
                        (release ? " " : ""),
                        (release ? release : ""),
                        (codename ? " (" : ""),
                        (codename ? codename : ""),
                        (codename ? ")" : ""));

  return NULL;
}

/* Read /etc/os-release, as documented here:
 * http://www.freedesktop.org/software/systemd/man/os-release.html
 */
static const char *
systemd_release(apr_pool_t *pool)
{
  svn_error_t *err;
  svn_stream_t *stream;

  /* Open the file. */
  err = svn_stream_open_readonly(&stream, "/etc/os-release", pool, pool);
  if (err && APR_STATUS_IS_ENOENT(err->apr_err))
    {
      svn_error_clear(err);
      err = svn_stream_open_readonly(&stream, "/usr/lib/os-release", pool,
                                     pool);
    }
  if (err)
    {
      svn_error_clear(err);
      return NULL;
    }

  /* Look for the PRETTY_NAME line. */
  while (TRUE)
    {
      svn_stringbuf_t *line;
      svn_boolean_t eof;

      err = svn_stream_readline(stream, &line, "\n", &eof, pool);
      if (err)
        {
          svn_error_clear(err);
          return NULL;
        }

      if (!strncmp(line->data, "PRETTY_NAME=", 12))
        {
          svn_stringbuf_t *release_name;

          /* The value may or may not be enclosed by double quotes.  We don't
           * attempt to strip them. */
          release_name = svn_stringbuf_create(line->data + 12, pool);
          svn_error_clear(svn_stream_close(stream));
          svn_stringbuf_strip_whitespace(release_name);
          return release_name->data;
        }

      if (eof)
        break;
    }

  /* The file did not contain a PRETTY_NAME line. */
  svn_error_clear(svn_stream_close(stream));
  return NULL;
}

/* Read the whole contents of a file. */
static svn_stringbuf_t *
read_file_contents(const char *filename, apr_pool_t *pool)
{
  svn_error_t *err;
  svn_stringbuf_t *buffer;

  err = svn_stringbuf_from_file2(&buffer, filename, pool);
  if (err)
    {
      svn_error_clear(err);
      return NULL;
    }

  return buffer;
}

/* Strip everything but the first line from a stringbuf. */
static void
stringbuf_first_line_only(svn_stringbuf_t *buffer)
{
  char *eol = strchr(buffer->data, '\n');
  if (eol)
    {
      *eol = '\0';
      buffer->len = 1 + eol - buffer->data;
    }
  svn_stringbuf_strip_whitespace(buffer);
}

/* Look at /etc/redhat_release to detect RHEL/Fedora/CentOS. */
static const char *
redhat_release(apr_pool_t *pool)
{
  svn_stringbuf_t *buffer = read_file_contents("/etc/redhat-release", pool);
  if (buffer)
    {
      stringbuf_first_line_only(buffer);
      return buffer->data;
    }
  return NULL;
}

/* Look at /etc/SuSE-release to detect non-LSB SuSE. */
static const char *
suse_release(apr_pool_t *pool)
{
  const char *release = NULL;
  const char *codename = NULL;

  svn_stringbuf_t *buffer = read_file_contents("/etc/SuSE-release", pool);
  svn_stringbuf_t *line;
  svn_stream_t *stream;
  svn_boolean_t eof;
  svn_error_t *err;
  if (!buffer)
      return NULL;

  stream = svn_stream_from_stringbuf(buffer, pool);
  err = svn_stream_readline(stream, &line, "\n", &eof, pool);
  if (err || eof)
    {
      svn_error_clear(err);
      return NULL;
    }

  svn_stringbuf_strip_whitespace(line);
  release = line->data;

  for (;;)
    {
      const char *key;

      err = svn_stream_readline(stream, &line, "\n", &eof, pool);
      if (err || eof)
        {
          svn_error_clear(err);
          break;
        }

      key = stringbuf_split_key(line, '=');
      if (!key)
        continue;

      if (0 == strncmp(key, "CODENAME", 8))
        codename = line->data;
    }

  return apr_psprintf(pool, "%s%s%s%s",
                      release,
                      (codename ? " (" : ""),
                      (codename ? codename : ""),
                      (codename ? ")" : ""));
}

/* Look at /etc/debian_version to detect non-LSB Debian. */
static const char *
debian_release(apr_pool_t *pool)
{
  svn_stringbuf_t *buffer = read_file_contents("/etc/debian_version", pool);
  if (!buffer)
      return NULL;

  stringbuf_first_line_only(buffer);
  return apr_pstrcat(pool, "Debian ", buffer->data, SVN_VA_NULL);
}

/* Try to find the Linux distribution name, or return info from uname. */
static const char *
linux_release_name(apr_pool_t *pool)
{
  const char *uname_release = release_name_from_uname(pool);

  /* Try anything that has /usr/bin/lsb_release.
     Covers, for example, Debian, Ubuntu and SuSE.  */
  const char *release_name = lsb_release(pool);

  /* Try the systemd way (covers Arch). */
  if (!release_name)
    release_name = systemd_release(pool);

  /* Try RHEL/Fedora/CentOS */
  if (!release_name)
    release_name = redhat_release(pool);

  /* Try Non-LSB SuSE */
  if (!release_name)
    release_name = suse_release(pool);

  /* Try non-LSB Debian */
  if (!release_name)
    release_name = debian_release(pool);

  if (!release_name)
    return uname_release;

  if (!uname_release)
    return release_name;

  return apr_psprintf(pool, "%s [%s]", release_name, uname_release);
}
#endif /* __linux__ */

#endif //OSBUILD_LINUX_VERSION_H
