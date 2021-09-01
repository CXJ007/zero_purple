#include "my_gui.h"
#include "nets.h"


#define GPIO_PWM_OUT 26


LV_FONT_DECLARE(my_font_0);
LV_FONT_DECLARE(my_font_1);
LV_IMG_DECLARE(background0);
LV_IMG_DECLARE(code0); //与心知天气对应
LV_IMG_DECLARE(code4);
LV_IMG_DECLARE(code6);
LV_IMG_DECLARE(code7);
LV_IMG_DECLARE(code9);
LV_IMG_DECLARE(code10);
LV_IMG_DECLARE(code11);
LV_IMG_DECLARE(code12);
LV_IMG_DECLARE(code13);
LV_IMG_DECLARE(code16);
LV_IMG_DECLARE(code18);
LV_IMG_DECLARE(code19);
LV_IMG_DECLARE(code20);
LV_IMG_DECLARE(code21);
LV_IMG_DECLARE(code24);
LV_IMG_DECLARE(code25);
LV_IMG_DECLARE(code30);
LV_IMG_DECLARE(code31);
LV_IMG_DECLARE(code32);
LV_IMG_DECLARE(code37);
LV_IMG_DECLARE(code99);

extern lv_indev_t * indev_keypad;
lv_group_t * group;
lv_obj_t* img_bg; //background
lv_obj_t *tileview;
lv_obj_t* tile0;
lv_obj_t* tile1;
lv_obj_t* tile2;
lv_obj_t* tile3;


lv_obj_t* labe0; //time
lv_obj_t* labe1; //day
lv_obj_t* img0;
lv_obj_t* labe2;
lv_obj_t* img1;
lv_obj_t* labe3;
lv_obj_t* img2;
lv_obj_t* labe4;
lv_obj_t* btn0; //set
lv_style_t* style_btn0;
lv_obj_t* roller0;
lv_style_t *style_roller0, *style_roller1,*style_arc0,*style_arc1;
lv_obj_t* labe5;
lv_obj_t* labe6;
lv_obj_t* arc0;
lv_obj_t* labe7;

extern SemaphoreHandle_t Startnet;
extern QueueHandle_t Queuedata;
infor *guiinfor;//所有信息
char timebuff[20];
char yearbuff[20];
char fd0buff[20];
char fd1buff[20];
char fd2buff[20];
char temphum0[40];
char temphum1[40];
char brightnum[20];
float bright[2] = {50,0};
int8_t brightflag[2];

static void event_handler(lv_obj_t* obj, lv_event_t event);



void tftpwminit(void)
{
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM_OUT);
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    //frequency = 500Hz,
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings
}

void tftpwmset(float duty)
{
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_UNIT_0, MCPWM_OPR_A, duty);
}

static void Time0_lv_tick_inc(void *p)
{
    static uint16_t num,flag;
    lv_tick_inc(1); //1MS
    num++;
    if(num >= 1000){
        if(flag == 0) flag = 1;
        else flag = 0;
        num = 0;
        guiinfor->flag[0] = 1;
        guiinfor->time[2]++;
        if(guiinfor->time[2] >= 60){
            guiinfor->time[2] = 0;
            guiinfor->time[1]++;
            if(guiinfor->time[1] >= 60){
                guiinfor->time[1] = 0;
                guiinfor->time[0]++;
                if(bright[1]<5) bright[1]++;//自动熄屏
                if(guiinfor->time[0] >= 23){
                    guiinfor->time[0] = 0;
                }
            }
        }
        if(flag == 0){
            if(guiinfor->time[0]>9 && guiinfor->time[1]>9)sprintf(timebuff,"#f62c51 %d:%d",guiinfor->time[0],guiinfor->time[1]);
            else if(guiinfor->time[0]<=9 && guiinfor->time[1]>9)sprintf(timebuff,"#f62c51 0%d:%d",guiinfor->time[0],guiinfor->time[1]);
            else if(guiinfor->time[0]>9 && guiinfor->time[1]<=9)sprintf(timebuff,"#f62c51 %d:0%d",guiinfor->time[0],guiinfor->time[1]);
            else if(guiinfor->time[0]<=9 && guiinfor->time[1]<=9)sprintf(timebuff,"#f62c51 0%d:0%d",guiinfor->time[0],guiinfor->time[1]);
        }else{
            if(guiinfor->time[0]>9 && guiinfor->time[1]>9)sprintf(timebuff,"#f62c51 %d %d",guiinfor->time[0],guiinfor->time[1]);
            else if(guiinfor->time[0]<=9 && guiinfor->time[1]>9)sprintf(timebuff,"#f62c51 0%d %d",guiinfor->time[0],guiinfor->time[1]);
            else if(guiinfor->time[0]>9 && guiinfor->time[1]<=9)sprintf(timebuff,"#f62c51 %d 0%d",guiinfor->time[0],guiinfor->time[1]);
            else if(guiinfor->time[0]<=9 && guiinfor->time[1]<=9)sprintf(timebuff,"#f62c51 0%d 0%d",guiinfor->time[0],guiinfor->time[1]);
        }
        //printf("%d   %d  %d   %s\n",guiinfor->time[0],guiinfor->time[1],guiinfor->time[2],timebuff);
    }
}

static void time0_creat(void)
{
    esp_timer_handle_t time_handle0;
    esp_timer_create_args_t time_args0 = {
        .callback = &Time0_lv_tick_inc,
        .name = "lv_heart"};
    esp_timer_create(&time_args0, &time_handle0);
    esp_timer_start_periodic(time_handle0, 1000);
}

static void Gui_Btn0(void)
{
    
    btn0 = lv_btn_create(tile2, NULL);
    lv_obj_set_size(btn0, 80, 40);
    lv_obj_align(btn0, tile2, LV_ALIGN_CENTER, 0, 0);
    style_btn0 = lv_mem_alloc(sizeof(lv_style_t));
    lv_style_init(style_btn0);
    lv_style_set_radius(style_btn0, LV_STATE_DEFAULT, 20);
    lv_style_set_bg_opa(style_btn0, LV_STATE_FOCUSED|LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(style_btn0, LV_STATE_DEFAULT, LV_COLOR_MAKE(0XD0,0X88,0XD9));
    lv_style_set_border_color(style_btn0, LV_STATE_DEFAULT, LV_COLOR_MAKE(0XBD,0X10,0XCE));
    lv_style_set_border_width(style_btn0, LV_STATE_DEFAULT, 4);
    lv_style_set_text_color(style_btn0, LV_STATE_DEFAULT, LV_COLOR_MAKE(0XBD, 0X10, 0XCE));
    lv_style_set_bg_color(style_btn0, LV_STATE_FOCUSED, LV_COLOR_MAKE(0XD8, 0XC5, 0X64));
    lv_style_set_border_color(style_btn0, LV_STATE_FOCUSED, LV_COLOR_MAKE(0XDD, 0X64, 0X13));
    lv_style_set_border_width(style_btn0, LV_STATE_FOCUSED, 4);
    lv_style_set_text_color(style_btn0, LV_STATE_FOCUSED, LV_COLOR_MAKE(0XDD, 0X64, 0X13));
    lv_style_set_text_font(style_btn0, LV_STATE_DEFAULT, &my_font_1);
    lv_obj_t* labe5 = lv_label_create(btn0, NULL);
    lv_label_set_text(labe5, "SET");
    lv_obj_add_style(btn0, LV_BTN_PART_MAIN, style_btn0);
    lv_obj_set_event_cb(btn0, event_handler);
}

static void Gui_Roller0(void)
{
    style_roller0 = lv_mem_alloc(sizeof(lv_style_t));
    style_roller1 = lv_mem_alloc(sizeof(lv_style_t));
    lv_style_init(style_roller0);
    lv_style_init(style_roller1);
    lv_style_set_text_font(style_roller0, LV_STATE_DEFAULT, &my_font_1);
    lv_style_set_border_width(style_roller0, LV_STATE_FOCUSED, 0);
    lv_style_set_pad_left(style_roller0, LV_STATE_DEFAULT, 20);
    lv_style_set_pad_right(style_roller0, LV_STATE_DEFAULT, 20);
    lv_style_set_bg_color(style_roller0, LV_STATE_DEFAULT, LV_COLOR_MAKE(0XD8, 0XC5, 0X64));
    lv_style_set_bg_grad_color(style_roller0, LV_STATE_DEFAULT, LV_COLOR_MAKE(0XDD, 0X64, 0X13));
    lv_style_set_bg_grad_dir(style_roller0, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_color(style_roller1, LV_STATE_DEFAULT, LV_COLOR_MAKE(0XDE, 0X7D, 0X21));
    roller0 = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(roller0,
        "TIME\n"
        "DATA\n"
        "BRIGHT\n"
        "ESC",
        LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(roller0, 5);
    lv_obj_align(roller0, NULL, LV_ALIGN_CENTER, -25, 0);
    lv_obj_add_style(roller0, LV_ROLLER_PART_BG, style_roller0);
    lv_obj_add_style(roller0, LV_ROLLER_PART_SELECTED, style_roller1);
    lv_obj_set_event_cb(roller0, event_handler);
}

static void Gui_Arc0(void)
{
    style_arc0 = lv_mem_alloc(sizeof(lv_style_t));
    style_arc1 = lv_mem_alloc(sizeof(lv_style_t));
    lv_style_init(style_arc0);
    lv_style_init(style_arc1);
    lv_style_set_bg_opa(style_arc0, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_opa(style_arc0, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    arc0 = lv_arc_create(tile2,NULL);
    lv_arc_set_bg_angles(arc0, 0, 360);
    lv_obj_set_size(arc0, 70, 70);
    lv_arc_set_value(arc0,(int16_t)bright[0]);  //init bright 50
    lv_arc_set_adjustable(arc0, 1);
    lv_obj_align(arc0, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(arc0,LV_ARC_PART_BG, style_arc0);
    lv_style_set_line_width(style_arc1, LV_STATE_DEFAULT, 8);
    lv_style_set_line_color(style_arc1, LV_STATE_DEFAULT, LV_COLOR_MAKE(0XD8, 0X00, 0X00));
    lv_style_set_bg_color(style_arc1, LV_STATE_DEFAULT , LV_COLOR_MAKE(0XD8, 0X00, 0X00));
    lv_style_set_border_color(style_arc1, LV_STATE_DEFAULT|LV_STATE_FOCUSED, LV_COLOR_MAKE(0XD8, 0X00, 0X00));
    lv_obj_clear_state(arc0,LV_STATE_FOCUSED);
    lv_obj_add_style(arc0, LV_ARC_PART_INDIC, style_arc1);
    lv_obj_add_style(arc0, LV_ARC_PART_KNOB, style_arc1);
    
    lv_group_add_obj(group ,arc0);
    lv_obj_set_event_cb(arc0, event_handler);

    sprintf(brightnum,"#d80000 %d",(int16_t)bright[0]);
    labe7 = lv_label_create(arc0, NULL);
    lv_label_set_text(labe7, brightnum);
    lv_label_set_recolor(labe7, true);
    lv_style_set_text_font(style_arc1, LV_STATE_DEFAULT, &my_font_1);
    lv_obj_add_style(labe7, LV_LABEL_PART_MAIN, style_arc1);
    lv_obj_align(labe7, arc0, LV_ALIGN_CENTER, 0, 0);
}

void bright_change(void){
    if(brightflag[1] == 0){
        bright[0] += 0.05;
        if(bright[0] > 100)bright[0] = 100;
        sprintf(brightnum,"#d80000 %d",(int16_t)bright[0]);
        lv_label_set_text(labe7, brightnum);
        lv_arc_set_value(arc0,(int16_t)bright[0]);
        tftpwmset(bright[0]);
    }else if(brightflag[1] == 1){
        bright[0] -= 0.05;
        if(bright[0] < 0)bright[0] = 0;
        sprintf(brightnum,"#d80000 %d",(int16_t)bright[0]);
        lv_label_set_text(labe7, brightnum);
        lv_arc_set_value(arc0,(int16_t)bright[0]);
        tftpwmset(bright[0]);
    }else if(brightflag[1] == 2){
        brightflag[0] = 0;
        brightflag[1] = 0;
        lv_group_remove_obj(arc0);
        lv_obj_del_async(arc0); //lv_obj_del卡死
        lv_style_reset(style_arc0);
        lv_style_reset(style_arc1);
        lv_obj_move_foreground(roller0);
        lv_group_add_obj(group ,roller0);
    }
}

static void event_handler(lv_obj_t* obj, lv_event_t event)
{
    if(event == LV_EVENT_KEY){
        bright[1] = 0;
        const uint8_t *key = lv_event_get_data();
        if(obj == tileview){      //lv_tileview_get_tile_act(obj, &x, &y);不知道为啥用不了
            static int8_t x,y;
            if(x == 1){
                if(LV_KEY_ENTER == *key) y = 1;
                else if(LV_KEY_ESC == *key) y = 0;
            }
            if(y == 0){
                if(LV_KEY_RIGHT == *key) x++;
                else if(LV_KEY_LEFT == *key) x--;
            }
            if(x>2)x=2;
            if(x<0)x=0;
            //printf("%d   %d\n",x,y);
            lv_tileview_set_tile_act(obj, x, y, LV_ANIM_ON);
            if(x == 2) 
                lv_group_add_obj(group ,btn0);
            else
                lv_group_remove_obj(btn0);
        }else if((obj==btn0) && (*key==LV_KEY_ENTER)){
            Gui_Roller0();
            lv_group_remove_obj(tileview);
            lv_group_remove_obj(btn0);
            lv_obj_del_async(btn0);
            lv_style_reset(style_btn0);
            lv_group_add_obj(group ,roller0);
        }else if((obj==roller0) && (*key==LV_KEY_ENTER)){
            char buf[10] = {0};
            lv_roller_get_selected_str(obj, buf, sizeof(buf));
            if(strcmp(buf,"ESC") == 0){
                lv_group_remove_obj(roller0);
                lv_obj_del_async(roller0); //lv_obj_del卡死
                lv_style_reset(style_roller0);
                lv_style_reset(style_roller1);
                Gui_Btn0();
                lv_group_add_obj(group ,tileview);
                lv_group_add_obj(group ,btn0);
            }else if(strcmp(buf,"BRIGHT") == 0){
                lv_obj_move_background(roller0);
                lv_group_remove_obj(roller0);
                Gui_Arc0();
                brightflag[0] = 1;
            }
            
        }else if(obj == arc0){
            static uint8_t keychang,key1;
            if(key1 == *key) keychang = 0;
            else keychang = 1;
            key1 = *key;
            if(LV_KEY_ESC == *key){
                if(keychang == 0){
                    brightflag[1] = 0;
                }else{
                    brightflag[1] = 3;
                }
            }else if(LV_KEY_ENTER == *key){
                if(keychang == 0){
                    brightflag[1] = 1;
                }else{
                    brightflag[1] = 3;
                }
            }else if(LV_KEY_RIGHT == *key){
                brightflag[1] = 2;
                keychang = 0;
                key1 = 0;
            }else brightflag[1] = 3;//无效
        }
        
    }
    
}


// void lvgl_fs_test(void)
// {
//     lv_fs_file_t *fd = (lv_fs_file_t*)malloc(sizeof(lv_fs_file_t));
//     lv_fs_res_t res;
//     // const char a[] = "asdf";
//     char b[2] = {0};
//     res = lv_fs_open(fd, "H:/qweq.txt", LV_FS_MODE_RD);
//     if (res != LV_FS_RES_OK) {
//         printf("open ERROR\n");
//         return ;
//     }
//     // res = lv_fs_write(fd,(const void *)a,sizeof(a),NULL);
//     // if (res != LV_FS_RES_OK) {
//     //     printf("write ERROR\n");
//     //     return ;
//     // }
    //  res = lv_fs_read(fd,b,2,NULL);
//     if (res != LV_FS_RES_OK) {
//         lv_fs_close(fd);
//         printf("write ERROR\n");
//         return ;
//     }
//     printf("%s\n",b);
//     res = lv_fs_close(fd);
//     if (res != LV_FS_RES_OK) {
//         printf("close ERROR\n");
//         return ;
//     }
// }

void Gui_Init(void)
{
    lv_init();
    time0_creat();
    lv_port_disp_init();
    lv_port_indev_init();
    //lv_port_fs_init();

    //lvgl_fs_test();
}

void Gui_Start(void)
{
    group = lv_group_create();
    lv_indev_set_group(indev_keypad, group);

    static lv_style_t style0, style1;
    lv_style_init(&style0);
    lv_style_init(&style1);
    lv_style_set_text_font(&style0, LV_STATE_DEFAULT, &my_font_0);
    lv_style_set_text_font(&style1, LV_STATE_DEFAULT, &my_font_1);

    img_bg = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img_bg, &background0);
    lv_obj_align(img_bg, NULL, LV_ALIGN_CENTER, 0, 0);

    static lv_point_t valid_pos[] = { {0,0}, {1,0},{2,0},{1,1} };
    tileview = lv_tileview_create(lv_scr_act(), NULL);
    lv_tileview_set_valid_positions(tileview, valid_pos, 4);
    lv_tileview_set_edge_flash(tileview, true);
    lv_style_set_bg_opa(&style0, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_add_style(tileview, LV_PAGE_PART_BG, &style0);

    lv_style_set_border_width(&style0, LV_STATE_DEFAULT, 0);
    tile0 = lv_obj_create(tileview, NULL);
    lv_obj_set_size(tile0, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_style(tile0, LV_PAGE_PART_BG, &style0);
    lv_tileview_add_element(tileview, tile0);
    labe0 = lv_label_create(tile0, NULL); //time
    lv_label_set_recolor(labe0, true);
    //lv_label_set_text(labe0, "#0F00ff 4##ff00ff 2:##007acc 23");
    lv_label_set_text(labe0, timebuff);
    lv_obj_align(labe0, tile0, LV_ALIGN_CENTER, -10, -10);
    labe1 = lv_label_create(tile0, NULL);
    lv_label_set_recolor(labe1, true);
    sprintf(yearbuff,"#ff00ff %s/%s/%s",guiinfor->year,guiinfor->month,guiinfor->day);
    lv_obj_add_style(labe1, LV_LABEL_PART_MAIN, &style1);
    lv_obj_align(labe1, tile0, LV_ALIGN_IN_BOTTOM_MID, -18, -10);

    tile1 = lv_obj_create(tileview, tile0);
    lv_obj_set_pos(tile1, LV_HOR_RES, 0);
    lv_tileview_add_element(tileview, tile1);
    img0 = lv_img_create(tile1, NULL);
    //lv_img_set_src(img0,"H:/LV/SUN.bin");
    lv_img_set_src(img0, &code99);
    lv_obj_align(img0, tile1, LV_ALIGN_IN_TOP_LEFT, 10, 5);
    labe2 = lv_label_create(tile1, labe1);
    lv_label_set_text(labe2, fd0buff);
    lv_obj_align(labe2, tile1, LV_ALIGN_IN_BOTTOM_LEFT, 2, -10);
    img1 = lv_img_create(tile1, NULL);
    //lv_img_set_src(img1, "H:/LV/RAIN.bin");
    lv_img_set_src(img1, &code99);
    lv_obj_align(img1, tile1, LV_ALIGN_IN_TOP_MID, 0, 5);
    labe3 = lv_label_create(tile1, labe1);
    lv_label_set_text(labe3, fd1buff);
    lv_obj_align(labe3, tile1, LV_ALIGN_IN_BOTTOM_MID, -25, -10);
    img2 = lv_img_create(tile1, NULL);
    //lv_img_set_src(img2, "H:/LV/CLOUD.bin");
    lv_img_set_src(img2, &code99);
    lv_obj_align(img2, tile1, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    labe4 = lv_label_create(tile1, labe1);
    lv_label_set_text(labe4, fd2buff);
    lv_obj_align(labe4, tile1, LV_ALIGN_IN_BOTTOM_RIGHT, -50, -10);

    tile2 = lv_obj_create(tileview, tile0);
    lv_obj_set_pos(tile2, LV_HOR_RES*2, 0);
    lv_tileview_add_element(tileview, tile2);
    Gui_Btn0();

    tile3 = lv_obj_create(tileview, tile0);
    lv_obj_set_pos(tile3, LV_HOR_RES , LV_VER_RES);
    lv_tileview_add_element(tileview, tile3);
    labe5 = lv_label_create(tile3, labe1);
    lv_obj_add_style(labe5, LV_LABEL_PART_MAIN, &style1);
    lv_label_set_text(labe5,temphum0);
    lv_obj_align(labe5, tile3, LV_ALIGN_CENTER, -60, -16);
    labe6 = lv_label_create(tile3, labe1);
    lv_label_set_text(labe6,temphum1);
    lv_obj_align(labe6, labe5, LV_ALIGN_OUT_BOTTOM_MID, 0, 16);

    lv_group_add_obj(group ,tileview);
    lv_obj_set_event_cb(tileview, event_handler);

    tftpwminit();
    tftpwmset(bright[0]);
}

void weather_chose_shown(lv_obj_t * img,uint8_t num)
{
    switch(num){
        case 0:lv_img_set_src(img,&code0);break;
        case 4:lv_img_set_src(img,&code4);break;
        case 5:
        case 6:lv_img_set_src(img,&code6);break;
        case 7:
        case 8:lv_img_set_src(img,&code7);break;
        case 9:lv_img_set_src(img,&code9);break;
        case 10:lv_img_set_src(img,&code10);break;
        case 11:lv_img_set_src(img,&code11);break;
        case 12:lv_img_set_src(img,&code12);break;
        case 13:lv_img_set_src(img,&code13);break;
        case 14:
        case 15:
        case 16:lv_img_set_src(img,&code16);break;
        case 17:
        case 18:lv_img_set_src(img,&code18);break;
        case 19:lv_img_set_src(img,&code19);break;
        case 20:lv_img_set_src(img,&code20);break;
        case 21:lv_img_set_src(img,&code21);break;
        case 22:
        case 23:
        case 24:lv_img_set_src(img,&code24);break;
        case 25:lv_img_set_src(img,&code25);break;
        case 30:lv_img_set_src(img,&code30);break;
        case 31:lv_img_set_src(img,&code31);break;
        case 32:
        case 33:lv_img_set_src(img,&code32);break;
        case 37:lv_img_set_src(img,&code37);break;
        default:lv_img_set_src(img,&code99);
    }
}

void infor_init(infor* dat)
{
    char *a = "0000";
    char *b = "00";
    uint8_t i;
    memset(dat->weather,0,3);
    memset(dat->temperature,0,3);
    memset(dat->humidity,0,3);
    for(i=0;i<3;i++){
        strcpy(dat->fdata[i].month,b);
        strcpy(dat->fdata[i].day,b);
    }

    memset(dat->time,0,3);
    strcpy(dat->year,a);
    strcpy(dat->month,b);
    strcpy(dat->day,b);

    lv_label_set_text(labe0, timebuff);
    sprintf(yearbuff,"#ff00ff %s/%s/%s",guiinfor->year,guiinfor->month,guiinfor->day);
    lv_label_set_text(labe1,yearbuff);
    sprintf(fd0buff,"#ff00ff %s/%s",guiinfor->fdata[0].month,guiinfor->fdata[0].day);
    lv_label_set_text(labe2,fd0buff);
    sprintf(fd1buff,"#ff00ff %s/%s",guiinfor->fdata[1].month,guiinfor->fdata[1].day);
    lv_label_set_text(labe3,fd1buff);
    sprintf(fd2buff,"#ff00ff %s/%s",guiinfor->fdata[2].month,guiinfor->fdata[2].day);
    lv_label_set_text(labe4,fd2buff);
    sprintf(temphum0,"#ff00ff  T%d  T%d  T%d",guiinfor->temperature[0],guiinfor->temperature[1],guiinfor->temperature[2]);
    sprintf(temphum1,"#ff00ff  H%d  H%d  H%d",guiinfor->humidity[0],guiinfor->humidity[1],guiinfor->humidity[2]);
    lv_label_set_text(labe5,temphum0);
    lv_label_set_text(labe6,temphum1);

}

void vTaskGui( void * pvParameters )
{
    uint8_t i = 0;
    guiinfor = (infor*)malloc(sizeof(infor));
    memset(guiinfor,0,sizeof(infor));

    Gui_Init();
    Gui_Start();
    infor_init(guiinfor);
    // guiinfor->time[0] = 23;
    // guiinfor->time[1] = 59;
    // guiinfor->time[2] = 50;
    for( ;; )
    {
        xQueueReceive(Queuedata,guiinfor,0);
        if((guiinfor->time[0]==0) && (i==0)){  //0点更新 启动更新
            guiinfor->flag[2] = 1;
            i = 1;
        }else{
            guiinfor->flag[2] = 0;
        }
        if(guiinfor->flag[2]==1){ //启动 
            if(xSemaphoreGive(Startnet) == pdTRUE){ //信号量被释放
                guiinfor->flag[2] = 0;
                //printf("12321312321\n");
            }else{
                guiinfor->flag[2] = 1;
            }
        }
        
        if(guiinfor->flag[0] == 1){
            guiinfor->flag[0] = 0;
            lv_label_set_text(labe0, timebuff);
        }
        if(guiinfor->flag[1] == 1){
            guiinfor->flag[1] = 0;
            sprintf(yearbuff,"#ff00ff %s/%s/%s",guiinfor->year,guiinfor->month,guiinfor->day);
            lv_label_set_text(labe1,yearbuff);
        }
        if(guiinfor->flag[3] == 1){
            guiinfor->flag[3] = 0;
            sprintf(fd0buff,"#ff00ff %s/%s",guiinfor->fdata[0].month,guiinfor->fdata[0].day);
            lv_label_set_text(labe2,fd0buff);
            sprintf(fd1buff,"#ff00ff %s/%s",guiinfor->fdata[1].month,guiinfor->fdata[1].day);
            lv_label_set_text(labe3,fd1buff);
            sprintf(fd2buff,"#ff00ff %s/%s",guiinfor->fdata[2].month,guiinfor->fdata[2].day);
            lv_label_set_text(labe4,fd2buff);
            weather_chose_shown(img0,guiinfor->weather[0]);
            weather_chose_shown(img1,guiinfor->weather[1]);
            weather_chose_shown(img2,guiinfor->weather[2]);
            sprintf(temphum0,"#ff00ff  T%d  T%d  T%d",guiinfor->temperature[0],guiinfor->temperature[1],guiinfor->temperature[2]);
            sprintf(temphum1,"#ff00ff  H%d  H%d  H%d",guiinfor->humidity[0],guiinfor->humidity[1],guiinfor->humidity[2]);
            lv_label_set_text(labe5,temphum0);
            lv_label_set_text(labe6,temphum1);
        }
        if(brightflag[0] == 1)bright_change();
        if(bright[1]>=5) tftpwmset(0);
        // vTaskDelay(500/portTICK_PERIOD_MS);
        lv_task_handler();
        vTaskDelay(5/portTICK_PERIOD_MS);
        
    }
}
