/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2011 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

/**
 * SECTION:cd-client-sync
 * @short_description: Sync helpers for #CdClient
 *
 * These helper functions provide a simple way to use the async functions
 * in command line tools.
 *
 * See also: #CdClient
 */

#include "config.h"

#include <glib.h>
#include <gio/gio.h>

#include "cd-profile.h"
#include "cd-device.h"
#include "cd-client.h"
#include "cd-client-sync.h"

/* tiny helper to help us do the async operation */
typedef struct {
	GError		**error;
	GMainLoop	*loop;
	gboolean	 ret;
	CdProfile	*profile;
	CdDevice	*device;
	GPtrArray	*array;
} CdClientHelper;

static void
cd_client_connect_finish_sync (CdClient *client,
			       GAsyncResult *res,
			       CdClientHelper *helper)
{
	helper->ret = cd_client_connect_finish (client, res, helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_connect_sync:
 * @client: a #CdClient instance.
 * @cancellable: a #GCancellable or %NULL
 * @error: a #GError, or %NULL.
 *
 * Connects to the colord daemon.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: %TRUE for success, else %FALSE.
 *
 * Since: 0.1.0
 **/
gboolean
cd_client_connect_sync (CdClient *client,
			GCancellable *cancellable,
			GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;

	/* run async method */
	cd_client_connect (client, cancellable,
			   (GAsyncReadyCallback) cd_client_connect_finish_sync, &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.ret;
}

/**********************************************************************/

static void
cd_client_delete_profile_finish_sync (CdClient *client,
				      GAsyncResult *res,
				      CdClientHelper *helper)
{
	helper->ret = cd_client_delete_profile_finish (client, res, helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_delete_profile_sync:
 * @client: a #CdClient instance.
 * @profile: a #CdProfile.
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Deletes a color profile.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: %TRUE is the profile was deleted
 *
 * Since: 0.1.8
 **/
gboolean
cd_client_delete_profile_sync (CdClient *client,
			       CdProfile *profile,
			       GCancellable *cancellable,
			       GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;

	/* run async method */
	cd_client_delete_profile (client, profile, cancellable,
				  (GAsyncReadyCallback) cd_client_delete_profile_finish_sync, &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.ret;
}

/**********************************************************************/

static void
cd_client_delete_device_finish_sync (CdClient *client,
				      GAsyncResult *res,
				      CdClientHelper *helper)
{
	helper->ret = cd_client_delete_device_finish (client, res, helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_delete_device_sync:
 * @client: a #CdClient instance.
 * @device: a #CdDevice.
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Deletes a color device.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: %TRUE is the device was deleted
 *
 * Since: 0.1.8
 **/
gboolean
cd_client_delete_device_sync (CdClient *client,
			      CdDevice *device,
			      GCancellable *cancellable,
			      GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;

	/* run async method */
	cd_client_delete_device (client, device, cancellable,
				  (GAsyncReadyCallback) cd_client_delete_device_finish_sync, &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.ret;
}

/**********************************************************************/

static void
cd_client_find_profile_finish_sync (CdClient *client,
				    GAsyncResult *res,
				    CdClientHelper *helper)
{
	helper->profile = cd_client_find_profile_finish (client,
							 res,
							 helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_find_profile_sync:
 * @client: a #CdClient instance.
 * @id: id for the profile
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Finds a color profile from its id.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: A #CdProfile object, or %NULL for error
 *
 * Since: 0.1.0
 **/
CdProfile *
cd_client_find_profile_sync (CdClient *client,
			     const gchar *id,
			     GCancellable *cancellable,
			     GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;
	helper.profile = NULL;

	/* run async method */
	cd_client_find_profile (client, id, cancellable,
				(GAsyncReadyCallback) cd_client_find_profile_finish_sync,
				&helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.profile;
}

/**********************************************************************/

static void
cd_client_find_profile_by_filename_finish_sync (CdClient *client,
						GAsyncResult *res,
						CdClientHelper *helper)
{
	helper->profile = cd_client_find_profile_by_filename_finish (client,
								     res,
								     helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_find_profile_by_filename_sync:
 * @client: a #CdClient instance.
 * @filename: filename for the profile
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Finds a color profile from its filename.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: A #CdProfile object, or %NULL for error
 *
 * Since: 0.1.3
 **/
CdProfile *
cd_client_find_profile_by_filename_sync (CdClient *client,
					 const gchar *filename,
					 GCancellable *cancellable,
					 GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;
	helper.profile = NULL;

	/* run async method */
	cd_client_find_profile_by_filename (client, filename, cancellable,
					    (GAsyncReadyCallback) cd_client_find_profile_by_filename_finish_sync,
					    &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.profile;
}

/**********************************************************************/

static void
cd_client_create_profile_finish_sync (CdClient *client,
				      GAsyncResult *res,
				      CdClientHelper *helper)
{
	helper->profile = cd_client_create_profile_finish (client,
							   res,
							   helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_create_profile_sync:
 * @client: a #CdClient instance.
 * @id: identifier for the device
 * @scope: the scope of the profile
 * @properties: properties to set on the profile, or %NULL
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Creates a color profile.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: A #CdProfile object, or %NULL for error
 *
 * Since: 0.1.2
 **/
CdProfile *
cd_client_create_profile_sync (CdClient *client,
			       const gchar *id,
			       CdObjectScope scope,
			       GHashTable *properties,
			       GCancellable *cancellable,
			       GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;
	helper.profile = NULL;

	/* run async method */
	cd_client_create_profile (client, id, scope,
				  properties, cancellable,
				  (GAsyncReadyCallback) cd_client_create_profile_finish_sync,
				  &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.profile;
}

/**********************************************************************/

static void
cd_client_create_device_finish_sync (CdClient *client,
				      GAsyncResult *res,
				      CdClientHelper *helper)
{
	helper->device = cd_client_create_device_finish (client,
							   res,
							   helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_create_device_sync:
 * @client: a #CdClient instance.
 * @id: identifier for the device
 * @scope: the scope of the device
 * @properties: properties to set on the device, or %NULL
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Creates a color device.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: A #CdDevice object, or %NULL for error
 *
 * Since: 0.1.2
 **/
CdDevice *
cd_client_create_device_sync (CdClient *client,
			       const gchar *id,
			       CdObjectScope scope,
			       GHashTable *properties,
			       GCancellable *cancellable,
			       GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;
	helper.device = NULL;

	/* run async method */
	cd_client_create_device (client, id, scope,
				  properties, cancellable,
				  (GAsyncReadyCallback) cd_client_create_device_finish_sync,
				  &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.device;
}

/**********************************************************************/

static void
cd_client_get_devices_finish_sync (CdClient *client,
				   GAsyncResult *res,
				   CdClientHelper *helper)
{
	helper->array = cd_client_get_devices_finish (client,
						      res,
						      helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_get_devices_sync:
 * @client: a #CdClient instance.
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Get an array of the device objects.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: (transfer full): an array of #CdDevice objects,
 *		 free with g_ptr_array_unref()
 *
 * Since: 0.1.0
 **/
GPtrArray *
cd_client_get_devices_sync (CdClient *client,
			    GCancellable *cancellable,
			    GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;
	helper.array = NULL;

	/* run async method */
	cd_client_get_devices (client, cancellable,
			       (GAsyncReadyCallback) cd_client_get_devices_finish_sync,
			       &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.array;
}

/**********************************************************************/

static void
cd_client_get_profiles_finish_sync (CdClient *client,
				   GAsyncResult *res,
				   CdClientHelper *helper)
{
	helper->array = cd_client_get_profiles_finish (client,
						       res,
						       helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_get_profiles_sync:
 * @client: a #CdClient instance.
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Get an array of the profile objects.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: (transfer full): an array of #CdProfile objects,
 *		 free with g_ptr_array_unref()
 *
 * Since: 0.1.0
 **/
GPtrArray *
cd_client_get_profiles_sync (CdClient *client,
			     GCancellable *cancellable,
			     GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;
	helper.array = NULL;

	/* run async method */
	cd_client_get_profiles (client, cancellable,
			        (GAsyncReadyCallback) cd_client_get_profiles_finish_sync,
			        &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.array;
}

/**********************************************************************/

static void
cd_client_get_sensors_finish_sync (CdClient *client,
				   GAsyncResult *res,
				   CdClientHelper *helper)
{
	helper->array = cd_client_get_sensors_finish (client,
						      res,
						      helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_get_sensors_sync:
 * @client: a #CdClient instance.
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Get an array of the sensor objects.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: (transfer full): an array of #CdSensor objects,
 *		 free with g_ptr_array_unref()
 *
 * Since: 0.1.0
 **/
GPtrArray *
cd_client_get_sensors_sync (CdClient *client,
			    GCancellable *cancellable,
			    GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;
	helper.array = NULL;

	/* run async method */
	cd_client_get_sensors (client, cancellable,
			       (GAsyncReadyCallback) cd_client_get_sensors_finish_sync,
			       &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.array;
}

/**********************************************************************/

static void
cd_client_find_device_finish_sync (CdClient *client,
				   GAsyncResult *res,
				   CdClientHelper *helper)
{
	helper->device = cd_client_find_device_finish (client,
						       res,
						       helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_find_device_sync:
 * @client: a #CdClient instance.
 * @id: The device ID.
 * @cancellable: a #GCancellable or %NULL
 * @error: a #GError, or %NULL.
 *
 * Finds a color device.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: A #CdDevice object, or %NULL for error
 *
 * Since: 0.1.0
 **/
CdDevice *
cd_client_find_device_sync (CdClient *client,
			    const gchar *id,
			    GCancellable *cancellable,
			    GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;

	/* run async method */
	cd_client_find_device (client, id, cancellable,
			       (GAsyncReadyCallback) cd_client_find_device_finish_sync,
			       &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.device;
}

/**********************************************************************/

static void
cd_client_find_device_by_property_finish_sync (CdClient *client,
					       GAsyncResult *res,
					       CdClientHelper *helper)
{
	helper->device = cd_client_find_device_by_property_finish (client,
								   res,
								   helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_find_device_by_property_sync:
 * @client: a #CdClient instance.
 * @id: The device ID.
 * @cancellable: a #GCancellable or %NULL
 * @error: a #GError, or %NULL.
 *
 * Finds a color device that has a property value.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: A #CdDevice object, or %NULL for error
 *
 * Since: 0.1.8
 **/
CdDevice *
cd_client_find_device_by_property_sync (CdClient *client,
					const gchar *key,
					const gchar *value,
					GCancellable *cancellable,
					GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;

	/* run async method */
	cd_client_find_device_by_property (client, key, value, cancellable,
					   (GAsyncReadyCallback) cd_client_find_device_by_property_finish_sync,
					   &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.device;
}

/**********************************************************************/

static void
cd_client_get_standard_space_finish_sync (CdClient *client,
					  GAsyncResult *res,
					  CdClientHelper *helper)
{
	helper->profile = cd_client_get_standard_space_finish (client,
							       res,
							       helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_get_standard_space_sync:
 * @client: a #CdClient instance.
 * @standard_space: standard colorspace value
 * @cancellable: a #GCancellable, or %NULL
 * @error: a #GError, or %NULL
 *
 * Finds a standard colorspace.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: A #CdProfile object, or %NULL for error
 *
 * Since: 0.1.2
 **/
CdProfile *
cd_client_get_standard_space_sync (CdClient *client,
				   CdStandardSpace standard_space,
				   GCancellable *cancellable,
				   GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;
	helper.profile = NULL;

	/* run async method */
	cd_client_get_standard_space (client, standard_space, cancellable,
				      (GAsyncReadyCallback) cd_client_get_standard_space_finish_sync,
				      &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.profile;
}

/**********************************************************************/

static void
cd_client_get_devices_by_kind_finish_sync (CdClient *client,
					   GAsyncResult *res,
					   CdClientHelper *helper)
{
	helper->array = cd_client_get_devices_by_kind_finish (client,
							      res,
							      helper->error);
	g_main_loop_quit (helper->loop);
}

/**
 * cd_client_get_devices_by_kind_sync:
 * @client: a #CdClient instance.
 * @kind: a #CdDeviceKind, e.g. %CD_DEVICE_KIND_DISPLAY
 * @cancellable: a #GCancellable or %NULL
 * @error: a #GError, or %NULL.
 *
 * Get an array of the device objects of a specified kind.
 *
 * WARNING: This function is synchronous, and may block.
 * Do not use it in GUI applications.
 *
 * Return value: (transfer full): an array of #CdDevice objects,
 *		 free with g_ptr_array_unref()
 *
 * Since: 0.1.0
 **/
GPtrArray *
cd_client_get_devices_by_kind_sync (CdClient *client,
				    CdDeviceKind kind,
				    GCancellable *cancellable,
				    GError **error)
{
	CdClientHelper helper;

	/* create temp object */
	helper.loop = g_main_loop_new (NULL, FALSE);
	helper.error = error;

	/* run async method */
	cd_client_get_devices_by_kind (client, kind, cancellable,
				       (GAsyncReadyCallback) cd_client_get_devices_by_kind_finish_sync,
				       &helper);
	g_main_loop_run (helper.loop);

	/* free temp object */
	g_main_loop_unref (helper.loop);

	return helper.array;
}

/**********************************************************************/