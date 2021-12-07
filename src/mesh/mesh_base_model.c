/*
* Plantenna 2 node - bt mesh base_model
* File name:    base_model.c
* Author:       Frank Arts
* Date:         20-01-2021
* Version:      V1
* Version info
* - Created of file
* - Added file info + config and health server models + sensor client model
*/

/* EXTRA FILE INFO
*    This c file creates the base models config and health. It also includes the provisioning functionality.
*    To ad other models and/or elements, add the model header file under '//Includes' (do the same in <base_model.h>),
*        add the new model to the 'sig_models[]' array (create ven_models[] if it is a vendor model) under 'Composition',
*        and the new element to the 'elements[]' array under 'Composition'.
*    Node feature (relay, beacon, friend, gatt_proxy) can be changed in 'cfg_srv' under 'Configuration Server'.
*    For syntax, look at <bluetooth/mesh/access.h> and https://docs.zephyrproject.org/latest/reference/bluetooth/mesh/access.html
*
*    This base model can be used for both clients and servers. Note: Client and (Setup) Server most likely have different header files and models
*/

// Includes
#include "spms_libs.h"
#include "mesh_base_model.h"

#if defined(__SPMS_BT) && __SPMS_BT==1
    #include "mesh_sensor_setup_server.h"
    //#include "mesh_gen_onoff_server.h"
	// If needed, add more header files for (setup) server models here //
#else
    #include "mesh_sensor_client.h"
    //#include "mesh_gen_onoff_client.h"
	// If needed, add more header files for client models here //
#endif

/* BT MESH */
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <settings/settings.h>
#include <bluetooth/mesh/proxy.h>



// -------------------------------------------------------------------------------------------------------
// Provisioning functions
// --------------------------
// provisioning callback functions
static void attention_on(struct bt_mesh_model *model) {
    printk("attention_on()\n");
}

static void attention_off(struct bt_mesh_model *model) {
    printk("attention_off()\n");
}

static const struct bt_mesh_health_srv_cb health_srv_cb = {
    .attn_on = attention_on,
    .attn_off = attention_off,
};

// provisioning properties and capabilities
static int provisioning_output_pin(bt_mesh_output_action_t action, uint32_t number) {
    printk("OOB Number: %04d\n", number);
    return 0;
}

static void provisioning_complete(uint16_t net_idx, uint16_t addr) {
    printk("Provisioning completed\n");
}

static void provisioning_reset(void) {
    bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
}

// provisioning properties and capabilities
static const struct bt_mesh_prov prov = {
    .uuid = dev_uuid,
    .output_size = 4,
    .output_actions = BT_MESH_DISPLAY_NUMBER,
    .output_number = provisioning_output_pin,
    .complete = provisioning_complete,
    .reset = provisioning_reset,
};

// -------------------------------------------------------------------------------------------------------
// Configuration Server
// --------------------
// static struct bt_mesh_cfg cfg_srv = {
//     .relay = BT_MESH_RELAY_DISABLED,
//     .beacon = BT_MESH_BEACON_DISABLED,
//     .frnd = BT_MESH_FRIEND_NOT_SUPPORTED,
//     .gatt_proxy = BT_MESH_GATT_PROXY_ENABLED,
//     .default_ttl = 7,
//     /* 3 transmissions with 20ms interval */
//     .net_transmit = BT_MESH_TRANSMIT(2, 20),
// };

// -------------------------------------------------------------------------------------------------------
// Health Server
// -------------
BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);
static struct bt_mesh_health_srv health_srv = {
    .cb = &health_srv_cb,
};

// -------------------------------------------------------------------------------------------------------
// Composition
// -----------
// elements contain models (1 or more)
struct bt_mesh_model sig_models[] = {
    BT_MESH_MODEL_CFG_SRV,
    BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
    #if defined(__SPMS_BT) && __SPMS_BT == 1    //Node
        BT_MESH_MODEL(BT_MESH_MODEL_ID_SENSOR_SETUP_SRV, sensor_setup_srv_op, &sensor_setup_srv, NULL),
        BT_MESH_MODEL(BT_MESH_MODEL_ID_SENSOR_SRV, sensor_srv_op, &sensor_srv, NULL),
        //BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op, &gen_onoff_srv, NULL),
    #else                                       //Server
        BT_MESH_MODEL(BT_MESH_MODEL_ID_SENSOR_CLI, sensor_cli_op, &sensor_cli, NULL),
        //BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op, &gen_onoff_cli, NULL),
    #endif
    // If needed, add more models here //
};

// If needed, add ven_models[]  here //

// node contains elements (1 or more)
static struct bt_mesh_elem elements[] = {
    BT_MESH_ELEM(0, sig_models, BT_MESH_MODEL_NONE),
    // If needed, add more elements here //
};

// node (only 1, but can have multiple elements, which can have multiple models)
static const struct bt_mesh_comp comp = {
    .cid = 0xFFFF,
    .elem = elements,
    .elem_count = ARRAY_SIZE(elements),
};

// -------------------------------------------------------------------------------------------------------
// BT MESH init
// -----------
void spms_mesh_init(int err) {
    if (err) {
        printk("bt_enable init failed with err %d\n", err);
        return;
    }
    
    printk("Bluetooth initialised OK\n");
    
    // prov is a bt_mesh_prov struct and is declared in provisioning.c
    err = bt_mesh_init(&prov, &comp);
    
    if (err) {
        printk("bt_mesh_init failed with err %d\n", err);
        return;
    }
    
    printk("Mesh initialised OK\n");
    
    if (IS_ENABLED(CONFIG_SETTINGS)) {
        settings_load();
        printk("Settings loaded\n");
    }
    
    if (!bt_mesh_is_provisioned()) {
        printk("Node has not been provisioned - beaconing\n");
        bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
    }
    else {
        printk("Node has already been provisioned\n");
    }
    
    return;
}
