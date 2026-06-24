#include "IOC/IOC_SrvAPI.h"

#include <string.h>

#define IOC_MAX_SERVICES 16
#define IOC_MAX_LINKS 64

typedef struct {
  IOC_Bool_T in_use;
  IOC_SrvID_T id;
  IOC_SrvArgs_T args;
} IOC_ServiceSlot_T;

typedef struct {
  IOC_Bool_T in_use;
  IOC_LinkID_T id;
  IOC_SrvID_T srv_id;
  IOC_LinkUsage_T client_usage;
} IOC_LinkSlot_T;

static IOC_ServiceSlot_T g_services[IOC_MAX_SERVICES];
static IOC_LinkSlot_T g_links[IOC_MAX_LINKS];
static IOC_SrvID_T g_next_srv_id = 1U;
static IOC_LinkID_T g_next_link_id = 1U;

static IOC_Bool_T IOC__isUriEqual(const IOC_SrvURI_T *a,
                                  const IOC_SrvURI_T *b) {
  if (!a || !b || !a->pProtocol || !a->pHost || !a->pPath || !b->pProtocol ||
      !b->pHost || !b->pPath) {
    return IOC_FALSE;
  }

  return (strcmp(a->pProtocol, b->pProtocol) == 0 &&
          strcmp(a->pHost, b->pHost) == 0 && strcmp(a->pPath, b->pPath) == 0 &&
          a->Port == b->Port)
             ? IOC_TRUE
             : IOC_FALSE;
}

static IOC_LinkUsage_T IOC__complementaryUsage(IOC_LinkUsage_T usage) {
  switch (usage) {
  case IOC_LinkUsageDatSender:
    return IOC_LinkUsageDatReceiver;
  case IOC_LinkUsageDatReceiver:
    return IOC_LinkUsageDatSender;
  case IOC_LinkUsageEvtConsumer:
    return IOC_LinkUsageEvtProducer;
  case IOC_LinkUsageEvtProducer:
    return IOC_LinkUsageEvtConsumer;
  case IOC_LinkUsageCmdInitiator:
    return IOC_LinkUsageCmdExecutor;
  case IOC_LinkUsageCmdExecutor:
    return IOC_LinkUsageCmdInitiator;
  default:
    return IOC_LinkUsageUndefined;
  }
}

static IOC_ServiceSlot_T *IOC__findServiceById(IOC_SrvID_T id) {
  for (size_t i = 0; i < IOC_MAX_SERVICES; ++i) {
    if (g_services[i].in_use && g_services[i].id == id) {
      return &g_services[i];
    }
  }
  return NULL;
}

static IOC_ServiceSlot_T *IOC__findServiceByUri(const IOC_SrvURI_T *uri) {
  for (size_t i = 0; i < IOC_MAX_SERVICES; ++i) {
    if (g_services[i].in_use &&
        IOC__isUriEqual(&g_services[i].args.SrvURI, uri)) {
      return &g_services[i];
    }
  }
  return NULL;
}

static IOC_LinkSlot_T *IOC__findLinkById(IOC_LinkID_T id) {
  for (size_t i = 0; i < IOC_MAX_LINKS; ++i) {
    if (g_links[i].in_use && g_links[i].id == id) {
      return &g_links[i];
    }
  }
  return NULL;
}

IOC_Result_T IOC_onlineService(IOC_SrvID_pT pSrvID,
                               const IOC_SrvArgs_pT pSrvArgs) {
  if (!pSrvID || !pSrvArgs || !pSrvArgs->SrvURI.pProtocol ||
      !pSrvArgs->SrvURI.pHost || !pSrvArgs->SrvURI.pPath) {
    return IOC_RESULT_INVALID_PARAM;
  }

  if (IOC__findServiceByUri(&pSrvArgs->SrvURI) != NULL) {
    return IOC_RESULT_CONFLICT_SRVARGS;
  }

  for (size_t i = 0; i < IOC_MAX_SERVICES; ++i) {
    if (!g_services[i].in_use) {
      g_services[i].in_use = IOC_TRUE;
      g_services[i].id = g_next_srv_id++;
      g_services[i].args = *pSrvArgs;
      *pSrvID = g_services[i].id;
      return IOC_RESULT_SUCCESS;
    }
  }

  return IOC_RESULT_TOO_MANY_SERVICES;
}

IOC_Result_T IOC_offlineService(IOC_SrvID_T SrvID) {
  IOC_ServiceSlot_T *service = IOC__findServiceById(SrvID);
  if (!service) {
    return IOC_RESULT_NOT_EXIST_SERVICE;
  }

  for (size_t i = 0; i < IOC_MAX_LINKS; ++i) {
    if (g_links[i].in_use && g_links[i].srv_id == SrvID) {
      g_links[i].in_use = IOC_FALSE;
    }
  }

  service->in_use = IOC_FALSE;
  service->id = IOC_INVALID_SRV_ID;
  memset(&service->args, 0, sizeof(service->args));
  return IOC_RESULT_SUCCESS;
}

IOC_Result_T IOC_acceptClient(IOC_SrvID_T SrvID, IOC_LinkID_pT pLinkID,
                              const IOC_Options_pT pOption) {
  (void)SrvID;
  (void)pLinkID;
  (void)pOption;
  return IOC_RESULT_NOT_SUPPORT_MANUAL_ACCEPT;
}

IOC_Result_T IOC_connectService(IOC_LinkID_pT pLinkID,
                                const IOC_ConnArgs_pT pConnArgs,
                                const IOC_Options_pT pOption) {
  (void)pOption;

  if (!pLinkID || !pConnArgs || !pConnArgs->SrvURI.pProtocol ||
      !pConnArgs->SrvURI.pHost || !pConnArgs->SrvURI.pPath) {
    return IOC_RESULT_INVALID_PARAM;
  }

  IOC_ServiceSlot_T *service = IOC__findServiceByUri(&pConnArgs->SrvURI);
  if (!service) {
    return IOC_RESULT_NOT_EXIST_SERVICE;
  }

  IOC_LinkUsage_T required = IOC__complementaryUsage(pConnArgs->Usage);
  if (required == IOC_LinkUsageUndefined ||
      (service->args.UsageCapabilites & required) == 0U) {
    return IOC_RESULT_INCOMPATIBLE_USAGE;
  }

  if ((service->args.Flags & IOC_SRVFLAG_AUTO_ACCEPT) == 0U) {
    return IOC_RESULT_NOT_SUPPORT_MANUAL_ACCEPT;
  }

  for (size_t i = 0; i < IOC_MAX_LINKS; ++i) {
    if (!g_links[i].in_use) {
      g_links[i].in_use = IOC_TRUE;
      g_links[i].id = g_next_link_id++;
      g_links[i].srv_id = service->id;
      g_links[i].client_usage = pConnArgs->Usage;
      *pLinkID = g_links[i].id;
      return IOC_RESULT_SUCCESS;
    }
  }

  return IOC_RESULT_TOO_MANY_LINKS;
}

IOC_Result_T IOC_closeLink(IOC_LinkID_T LinkID) {
  IOC_LinkSlot_T *link = IOC__findLinkById(LinkID);
  if (!link) {
    return IOC_RESULT_NOT_EXIST_LINK;
  }

  link->in_use = IOC_FALSE;
  link->id = IOC_INVALID_LINK_ID;
  link->srv_id = IOC_INVALID_SRV_ID;
  link->client_usage = IOC_LinkUsageUndefined;
  return IOC_RESULT_SUCCESS;
}
