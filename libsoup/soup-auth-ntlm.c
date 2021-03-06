/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * soup-auth-ntlm.c: HTTP NTLM Authentication helper
 *
 * Copyright (C) 2007 Red Hat, Inc.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include "soup-auth-ntlm.h"
#include "soup-headers.h"
#include "soup-message.h"
#include "soup-misc.h"
#include "soup-uri.h"

static gboolean update (SoupAuth *auth, SoupMessage *msg, GHashTable *auth_params);
static GSList *get_protection_space (SoupAuth *auth, SoupURI *source_uri);
static void authenticate (SoupAuth *auth, const char *username, const char *password);
static gboolean is_authenticated (SoupAuth *auth);
static char *get_authorization (SoupAuth *auth, SoupMessage *msg);

typedef struct {
	char *username, *password;
} SoupAuthNTLMPrivate;
#define SOUP_AUTH_NTLM_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOUP_TYPE_AUTH_NTLM, SoupAuthNTLMPrivate))

G_DEFINE_TYPE (SoupAuthNTLM, soup_auth_ntlm, SOUP_TYPE_AUTH)

static void
soup_auth_ntlm_init (SoupAuthNTLM *ntlm)
{
}

static void
finalize (GObject *object)
{
	SoupAuthNTLMPrivate *priv = SOUP_AUTH_NTLM_GET_PRIVATE (object);

	g_free (priv->username);
	if (priv->password) {
		memset (priv->password, 0, strlen (priv->password));
		g_free (priv->password);
	}

	G_OBJECT_CLASS (soup_auth_ntlm_parent_class)->finalize (object);
}

static void
soup_auth_ntlm_class_init (SoupAuthNTLMClass *auth_ntlm_class)
{
	SoupAuthClass *auth_class = SOUP_AUTH_CLASS (auth_ntlm_class);
	GObjectClass *object_class = G_OBJECT_CLASS (auth_ntlm_class);

	g_type_class_add_private (auth_ntlm_class, sizeof (SoupAuthNTLMPrivate));

	auth_class->scheme_name = "NTLM";
	auth_class->strength = 3;

	auth_class->update = update;
	auth_class->get_protection_space = get_protection_space;
	auth_class->authenticate = authenticate;
	auth_class->is_authenticated = is_authenticated;
	auth_class->get_authorization = get_authorization;

	object_class->finalize = finalize;
}

SoupAuth *
soup_auth_ntlm_new (const char *realm, const char *host)
{
	SoupAuth *auth;

	auth = g_object_new (SOUP_TYPE_AUTH_NTLM,
			     SOUP_AUTH_REALM, realm,
			     SOUP_AUTH_HOST, host,
			     NULL);
	return auth;
}

static gboolean
update (SoupAuth *auth, SoupMessage *msg, GHashTable *auth_params)
{
	g_return_val_if_reached (FALSE);
}

static GSList *
get_protection_space (SoupAuth *auth, SoupURI *source_uri)
{
	g_return_val_if_reached (NULL);
}

static void
authenticate (SoupAuth *auth, const char *username, const char *password)
{
	SoupAuthNTLMPrivate *priv = SOUP_AUTH_NTLM_GET_PRIVATE (auth);

	g_return_if_fail (username != NULL);
	g_return_if_fail (password != NULL);

	priv->username = g_strdup (username);
	priv->password = g_strdup (password);
}

static gboolean
is_authenticated (SoupAuth *auth)
{
	return SOUP_AUTH_NTLM_GET_PRIVATE (auth)->password != NULL;
}

static char *
get_authorization (SoupAuth *auth, SoupMessage *msg)
{
	g_return_val_if_reached (NULL);
}

const char *
soup_auth_ntlm_get_username (SoupAuth *auth)
{
	SoupAuthNTLMPrivate *priv = SOUP_AUTH_NTLM_GET_PRIVATE (auth);

	return priv->username;
}

const char *
soup_auth_ntlm_get_password (SoupAuth *auth)
{
	SoupAuthNTLMPrivate *priv = SOUP_AUTH_NTLM_GET_PRIVATE (auth);

	return priv->password;
}
