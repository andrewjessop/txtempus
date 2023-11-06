#include <stdio.h>

// dependency package - libdrm-dev
#include <xf86drm.h>
#include <xf86drmMode.h>

#define DRM_MODE_CONNECTED 1
#define DRM_MODE_DISCONNECTED 2

int main(void)
{
	printf("Hello, world!\n");
	int avail;
	avail = drmAvailable();
	printf("DRM Available: %d\n", avail);

	int fd = drmOpen("vc4", NULL);
	printf("drmOpen: %d\n", fd);

	drmModeResPtr res;
	res = drmModeGetResources(fd);
	if (!res) {
		drmClose(fd);
		printf("couldn't load resources\n");
		return -1;
	}

	printf("Connectors count: %d\n", res->count_connectors);
	uint32_t id = res->connectors[0];
	printf("Connector 0 id: %d\n", id);

	/*
	drmVersionPtr drm_version;
	drmGetVersion(fd);
	printf("%d.%d.%d, %d\n", 
		drm_version->version_major,
		drm_version->version_minor,
		drm_version->version_patchlevel,
		drm_version->name_len
		);
	drmFreeVersion(drm_version);
	*/

	drmModeConnectorPtr drm_connector;
	drm_connector = drmModeGetConnector(fd, id);
	if (!drm_connector) {
		printf("failed to get Connector Mode\n");
		return -1;
	}
	printf("got connector mode\n");

	// connection status
	switch(drm_connector->connection) {
	case DRM_MODE_CONNECTED:
		printf("Connected\n");
		break;
	case DRM_MODE_DISCONNECTED:
		printf("Disconnected\n");
		break;
	default:
		printf("Connection unknown status\n");
	}
	

	// connector type
	switch(drm_connector->connector_type) {
		case DRM_MODE_CONNECTOR_HDMIA:
		case DRM_MODE_CONNECTOR_HDMIB:
			printf ("HDMI connector\n");
			break;
		case DRM_MODE_CONNECTOR_DVII:
		case DRM_MODE_CONNECTOR_DVID:
		case DRM_MODE_CONNECTOR_DVIA:
			printf("DVI connector\n");
			break;
		case DRM_MODE_CONNECTOR_Composite:
			printf("Composite connector\n");
			break;
		case DRM_MODE_CONNECTOR_VGA:
			printf("VGA connector\n");
			break;
		default:
			printf("other, or unknown display connector\n");
			break;
	}
	drmModeFreeResources(res);
	drmClose(fd);
	return 0;
}
