

#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"
#include "neural_network/NeuralNetwork.h"
#include "neural_network/Layer.h"
#include "neural_network/ECG.h"
#include "neural_network/Lista.h"
#include "dhcp/dhcp_setup.h"
#include <stdio.h>

ECG ecg = NULL;

char txt_result[100];
double nn_result;

GX_WINDOW * global_widget;

//extern GX_WINDOW_ROOT * p_window_root;

//static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old);
static void update_prompt_text(GX_WIDGET * p_widget, GX_RESOURCE_ID id, char* text);
//static void update_button_text_id(GX_WIDGET * p_widget, GX_RESOURCE_ID id, UINT string_id);

UINT window1_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);

    global_widget = widget;

    /*switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(BTN_START, GX_EVENT_CLICKED):
        update_prompt_text(widget->gx_widget_parent, TXT_IP, getIpText());
        //update_button_text_id(widget->gx_widget_parent, ID_WINDOWCHANGER, GX_STRING_ID_BUTTON_ENABLED);
        //update_prompt_text_id(widget->gx_widget_parent, ID_INSTRUCTIONS, GX_STRING_ID_INSTRUCT_BUTTON);
        break;
    case GX_SIGNAL(CK_ECG1, GX_EVENT_RADIO_SELECT):
        ecg = createECG(0.06,-0.435,2.03,-0.75,0.635,1.0055555555555555,-0.7166666666666667,0.044444444444444446);
        if(ecg == NULL){
            update_prompt_text(widget->gx_widget_parent, P_RESULT, "ECG NULL");
        }else{
            update_prompt_text(widget->gx_widget_parent, P_RESULT, "ECG106");
        }
        break;
    case GX_SIGNAL(CK_ECG2, GX_EVENT_RADIO_SELECT):
        ecg = createECG(-0.605,-0.19,0.4,-2.02,-1.185,0.8083333333333333,-1.4083333333333334,0.05555555555555555);
        if(ecg == NULL){
            update_prompt_text(widget->gx_widget_parent, P_RESULT, "ECG NULL");
        }else{
            update_prompt_text(widget->gx_widget_parent, P_RESULT, "ECG118");
        }
        break;
    case GX_SIGNAL(BTN_RUN, GX_EVENT_CLICKED):
        break;
    default:
        gx_window_event_process(widget, event_ptr);
        break;
    }*/

    return result;
}


static void update_prompt_text(GX_WIDGET * p_widget, GX_RESOURCE_ID id, char* text)
{
    GX_PROMPT * p_prompt = NULL;

    ssp_err_t err = (ssp_err_t)gx_widget_find(p_widget, (USHORT)id, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET**)&p_prompt);
    if (TX_SUCCESS == err)
    {
        gx_prompt_text_set(p_prompt, text);
    }
}


void setIP(char* ip){
    update_prompt_text(global_widget, P_IP, ip);
    gx_system_canvas_refresh();
}

void setConnection(){
    update_prompt_text(global_widget, P_WAIT, "Connected!");
    gx_system_canvas_refresh();
}

void setWaitConn(){
    update_prompt_text(global_widget, P_WAIT, "Waiting connection...");
    gx_system_canvas_refresh();
}

//show_window((GX_WINDOW*)&window2, (GX_WIDGET*)widget, true);
/*UINT window2_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);

    switch (event_ptr->gx_event_type){
        case GX_EVENT_PEN_UP:
            show_window((GX_WINDOW*)&window1, (GX_WIDGET*)widget, true);
            break;
        default:
            result = gx_window_event_process(widget, event_ptr);
            break;
    }
    return result;
}

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old)
{
    UINT err = GX_SUCCESS;

    if (!p_new->gx_widget_parent)
    {
        err = gx_widget_attach(p_window_root, p_new);
    }
    else
    {
        err = gx_widget_show(p_new);
    }

    gx_system_focus_claim(p_new);

    GX_WIDGET * p_old = p_widget;
    if (p_old && detach_old)
    {
        if (p_old != (GX_WIDGET*)p_new)
        {
            gx_widget_detach(p_old);
        }
    }

    return err;
}*/


/*static void update_button_text_id(GX_WIDGET * p_widget, GX_RESOURCE_ID id, UINT string_id)
{
    GX_TEXT_BUTTON * p_button = NULL;

    ssp_err_t err = (ssp_err_t)gx_widget_find(p_widget, (USHORT)id, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET**)&p_button);
    if (TX_SUCCESS == err)
    {
        gx_text_button_text_id_set(p_button, string_id);
    }
}*/
