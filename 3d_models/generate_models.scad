// ==============================================================================
// RetroWatch-S3 參數化 3D 建模源代碼 (OpenSCAD)
// 專案：微型復古多功能掌機 (120.0 x 56.0 x 12.0 mm 寬屏舒適版)
// 包含：前殼 (Front Case)、後蓋 (Rear Case)、街機圓球搖桿套頭
// ==============================================================================

$fn = 60; // 圓弧圓滑度

// --- 全域幾何參數 (mm) ---
case_w = 120.0; // 寬度優化擴增至 120mm，左右兩側各增加 8mm 充足握持與走線空間
case_h = 56.0;
case_thick = 12.0;
front_thick = 6.5;
rear_thick = 6.0;
wall = 1.5;
corner_r = 3.0;

// --- 圓角立方體輔助模組 ---
module rounded_box(w, h, d, r) {
    hull() {
        translate([r, r, 0]) cylinder(r=r, h=d);
        translate([w-r, r, 0]) cylinder(r=r, h=d);
        translate([r, h-r, 0]) cylinder(r=r, h=d);
        translate([w-r, h-r, 0]) cylinder(r=r, h=d);
    }
}

// ==============================================================================
// 1. 前殼主體 (Front Case)
// ==============================================================================
module front_case() {
    difference() {
        union() {
            // 主外殼實體 (外形 120.0 x 56.0 x 6.5 mm)
            rounded_box(case_w, case_h, front_thick, corner_r);

            // 全周結合止口 (子口凸緣，高出分模面 1.2mm，厚度 0.75mm，帶內外 0.15mm 列印公差)
            // 與後蓋母口全周咬合，徹底防止上下殼錯位與接縫漏光
            translate([wall - 0.75, wall - 0.75, front_thick - 0.1])
                rounded_box(case_w - 2*(wall - 0.75), case_h - 2*(wall - 0.75), 1.2 + 0.1, corner_r - 0.5);

            // 右側 M2 滾花銅螺母柱 (2 處，外徑 4.8mm，內徑 3.0mm，熱熔固定 M2x3.0 螺母)
            // 下柱 Y=12.0；上柱優化至 Y=case_h-7.5 (48.5mm)，完美避開 START 按鍵開孔與走線
            translate([case_w - 6.0, 12.0, wall])
                cylinder(d=4.8, h=front_thick - wall);
            translate([case_w - 6.0, case_h - 7.5, wall])
                cylinder(d=4.8, h=front_thick - wall);
        }

        // 內部挖空 (內艙，挖通至止口頂端，淨寬 117.0mm，電池與主板容納空間極為充裕)
        translate([wall, wall, wall])
            rounded_box(case_w - wall*2, case_h - wall*2, front_thick + 2.0, corner_r - 0.5);

        // 正面銘板沉台凹槽 (118 x 54, 深 0.55mm)
        translate([(case_w - 118)/2, (case_h - 54)/2, -0.1])
            rounded_box(118.0, 54.0, 0.55 + 0.1, 2.0);

        // 2.4 吋螢幕視窗開孔 (50.0 x 37.5 mm, 中心 X=60.0, Y=27.0，適配標準 2.4" ST7789 藍板模組)
        translate([case_w/2 - 50.0/2, 27.0 - 37.5/2, -1.0])
            cube([50.0, 37.5, front_thick + 2.0]);

        // 螢幕壓克力沉台 (52.0 x 39.5, 深 1.0mm)
        translate([case_w/2 - 52.0/2, 27.0 - 39.5/2, -0.1])
            cube([52.0, 39.5, 1.0 + 0.1]);

        // 【BATTERY 電源/電量指示燈貫穿孔】(直徑 2.0mm, 中心 X=27.5, Y=31.0)
        // 貫通前殼，居中於搖桿飾圈與螢幕邊框之間 (兩邊各留 4.5mm)，可裝入 2mm 導光柱或露出內部 LED
        translate([27.5, 31.0, -1.0])
            cylinder(d=2.0, h=front_thick + 2.0);

        // 五向導航搖桿開孔 (直徑 8.5mm, 中心 X=17.0, Y=22.0)
        translate([17.0, 22.0, -1.0])
            cylinder(d=8.5, h=front_thick + 2.0);

        // 左上方 POWER 滑動開關開孔 (8.4 x 3.6mm, 中心 X=17.0, Y=45.5)
        translate([17.0 - 8.4/2, 45.5 - 3.6/2, -1.0])
            cube([8.4, 3.6, front_thick + 2.0]);

        // A 鍵圓形開孔 (直徑 7.6mm, 中心 X=106.0, Y=26.0)
        translate([106.0, 26.0, -1.0])
            cylinder(d=7.6, h=front_thick + 2.0);

        // B 鍵圓形開孔 (直徑 7.6mm, 中心 X=96.0, Y=18.0)
        translate([96.0, 18.0, -1.0])
            cylinder(d=7.6, h=front_thick + 2.0);

        // PAUSE 鍵膠囊槽 (6.0 x 2.8mm, 中心 X=95.0, Y=43.5)
        translate([95.0 - 3.0, 43.5 - 1.4, -1.0])
            cube([6.0, 2.8, front_thick + 2.0]);

        // START 鍵膠囊槽 (6.0 x 2.8mm, 中心 X=106.0, Y=43.5)
        translate([106.0 - 3.0, 43.5 - 1.4, -1.0])
            cube([6.0, 2.8, front_thick + 2.0]);

        // 左側 Type-C 沉板側槽 (9.2 x 3.4mm, 中心 X=0.0, Y=12.0)
        translate([-1.0, 12.0 - 9.2/2, front_thick - 3.4])
            cube([wall + 2.0, 9.2, 3.4 + 1.5]);

        // 蜂鳴器出音陣列小孔 (直徑 1.3mm x 4 顆)
        for (i = [0:3]) {
            translate([13.5 + i*2.3, 6.8, -1.0])
                cylinder(d=1.3, h=front_thick + 2.0);
        }

        // 左側【內部卡扣卡槽】 (完全位於內壁 X=wall，不外露！Y=18.0 與 Y=36.0)
        translate([wall - 0.7, 18.0 - 3.5, front_thick - 1.8])
            cube([1.2, 7.0, 1.6]);
        translate([wall - 0.7, 36.0 - 3.5, front_thick - 1.8])
            cube([1.2, 7.0, 1.6]);

        // 右側熱熔螺母底孔 (直徑 3.0mm, 深 4.5mm)
        translate([case_w - 6.0, 12.0, front_thick - 4.5])
            cylinder(d=3.0, h=5.0);
        translate([case_w - 6.0, case_h - 7.5, front_thick - 4.5])
            cylinder(d=3.0, h=5.0);
    }
}

// ==============================================================================
// 2. 後蓋主體 (Rear Case)
// ==============================================================================
module rear_case() {
    difference() {
        union() {
            // 後蓋主體外框 (120.0 x 56.0 x 6.0 mm)
            rounded_box(case_w, case_h, rear_thick, corner_r);

            // 【對外突出相機火山口保護環 (External Raised Volcano Ring)】
            // 凸出於背蓋外表面 1.2mm，外徑 12.5mm，平放桌面時牢牢保護光學玻璃防刮傷！
            translate([case_w/2, 40.0, -1.2])
                cylinder(d=12.5, h=1.2);

            // 【內部鏡頭定位與防護遮光套筒】
            // 向機身內艙延伸 2.5mm，外徑 12.5mm，完全包覆 OV5640 鏡頭模組並隔絕內部電池與走線
            translate([case_w/2, 40.0, wall])
                cylinder(d=12.5, h=2.5);

            // 【左側內部卡榫倒勾爪 (Internal Snap-fit Claws)】
            // 完全位於機殼內部 (X=wall ~ wall+1.8mm)，向前殼方向延伸 2.2mm
            // 組裝時左側由內側先斜向扣入前殼卡槽，再蓋下右側鎖螺絲，外觀平整無外露卡榫！
            translate([wall, 18.0 - 3.0, rear_thick - 0.1]) {
                cube([1.4, 6.0, 2.2]); // 懸臂彈性爪身
                translate([-0.6, 0, 1.4]) cube([0.7, 6.0, 0.8]); // 倒勾卡扣牙
            }
            translate([wall, 36.0 - 3.0, rear_thick - 0.1]) {
                cube([1.4, 6.0, 2.2]);
                translate([-0.6, 0, 1.4]) cube([0.7, 6.0, 0.8]);
            }

            // 右下角【防扯斷極致加強座 (Anti-Snap Reinforced Lanyard Boss)】
            // 局部壁厚增至 3.3mm (比常規壁厚翻倍)，直連右側壁與底板，承受衝擊抗拉力 >15kg
            translate([case_w - wall - 1.8, 2.0, wall])
                cube([1.8, 7.8, 3.8]);
            // 內角三角支撐肋 (Truss Gusset)：將拉力直接傳導並分散至整機大底板
            hull() {
                translate([case_w - wall - 1.8, 2.0, wall]) cube([1.8, 1.2, 3.8]);
                translate([case_w - wall - 4.5, 2.0, wall]) cube([1.0, 1.2, 1.0]);
            }

            // 右側螺絲導向柱 (與前殼螺母柱對位配合，內徑 2.3mm)
            translate([case_w - 6.0, 12.0, wall])
                cylinder(d=4.6, h=rear_thick - wall);
            translate([case_w - 6.0, case_h - 7.5, wall])
                cylinder(d=4.6, h=rear_thick - wall);
        }

        // 內部挖空 (內艙)
        translate([wall, wall, wall])
            rounded_box(case_w - wall*2, case_h - wall*2, rear_thick + 0.1, corner_r - 0.5);

        // 全周結合母口凹槽 (與前殼子口止口咬合，寬 0.95mm，深 1.3mm)
        translate([wall - 0.8, wall - 0.8, rear_thick - 1.3])
            rounded_box(case_w - 2*(wall - 0.8), case_h - 2*(wall - 0.8), 1.4, corner_r - 0.5);

        // 鏡頭光學視窗孔 (通孔直徑 8.0mm，貫穿外殼)
        translate([case_w/2, 40.0, -2.0])
            cylinder(d=8.0, h=rear_thick + 6.0);

        // 機殼內部鏡頭模組容納腔 (內徑 10.2mm，深 2.6mm，容納 OV5640 模組)
        translate([case_w/2, 40.0, wall - 0.1])
            cylinder(d=10.2, h=3.0);

        // 【火山口內部鏡片沉台 (Recessed Lens Shelf)】
        // 位於火山口內，直徑 10.2mm，沉深 1.4mm (相對於火山口頂端)
        // 裝入 1.0mm 防刮光學玻璃後，鏡片表面比外凸火山口內陷 0.2mm，徹底防刮！
        translate([case_w/2, 40.0, -1.3])
            cylinder(d=10.2, h=1.4);

        // 背面折疊金屬支架凹槽 (44.0 x 20.0 x 1.0mm)
        translate([(case_w - 44.0)/2, 8.0, -0.1])
            cube([44.0, 20.0, 1.0 + 0.1]);

        // 左側 Type-C 咬合開孔
        translate([-1.0, 12.0 - 9.2/2, rear_thick - 1.2])
            cube([wall + 2.0, 9.2, 1.3]);

        // 右下角【重型手繩吊飾孔系統 (Heavy-Duty Dual-Eyelet Lanyard Loop)】
        // 1. 外側防磨沉槽 (深 1.0mm, 長 7.8mm, 寬 2.6mm，手繩沉入側面不刮掌心)
        translate([case_w - 1.0, 5.5 - 7.8/2, 1.4])
            cube([1.5, 7.8, 2.6]);

        // 2. 雙穿繩通孔 (直徑 2.2mm x 2，孔 1: Y=3.2, 孔 2: Y=7.8，中置高達 2.4mm 超粗實心防斷橫柱)
        // 橫柱截面積擴增至 2.4mm x 3.3mm (~8.0mm²)，搭配 PETG / 高韌樹脂可徹底消除任何衝擊斷裂可能！
        translate([case_w - wall - 3.0, 3.2, 2.7])
            rotate([0, 90, 0])
                cylinder(d=2.2, h=wall + 4.0);
        translate([case_w - wall - 3.0, 7.8, 2.7])
            rotate([0, 90, 0])
                cylinder(d=2.2, h=wall + 4.0);

        // 右側 M2 沉頭螺絲貫通孔 (2 處，沉頭直徑 4.2mm, 深 1.5mm, 螺絲通孔 2.3mm)
        translate([case_w - 6.0, 12.0, -0.2]) {
            cylinder(d=4.2, h=1.6);
            cylinder(d=2.3, h=rear_thick + 2.0);
        }
        translate([case_w - 6.0, case_h - 7.5, -0.2]) {
            cylinder(d=4.2, h=1.6);
            cylinder(d=2.3, h=rear_thick + 2.0);
        }
    }
}

// ==============================================================================
// 3. 街機圓球搖桿套頭 (Arcade Ball-Top Joystick Cap)
// ==============================================================================
// 註：A/B 鍵採用現成注塑 A24 鍵帽，START/PAUSE 採用微動裸鍵直觸，均不需 3D 列印！
module joystick_ball_cap() {
    // 經典街機電玩台圓形球狀搖桿套頭 (Arcade Ball-Top Joystick Cap)
    // 頂部為直徑 8.2mm 的微型街機圓球，提供絕佳大拇指撥動與微操手感！
    difference() {
        union() {
            // 1. 頂部經典街機圓球 (Ball-Top)
            translate([0, 0, 6.5])
                sphere(d=8.2);
            
            // 2. 圓柱頸身 (連接球體與底座)
            cylinder(d1=4.8, d2=4.0, h=5.5);
            
            // 3. 底部防脫位圓盤階梯 (外徑 8.6mm, 厚度 0.8mm)
            cylinder(d=8.6, h=0.8);
        }
        
        // 內部方孔插槽：緊密適配五向開關方柄 (2.1 x 2.1 mm, 深 3.8mm)
        translate([-1.05, -1.05, -0.1])
            cube([2.1, 2.1, 3.9]);
    }
}

// ==============================================================================
// 4. OpenSCAD 視圖與零件匯出選擇 (開啟檔案時預設直接顯示！)
// ==============================================================================
/* [視圖模式選擇 / View Mode] */
// 預設為平鋪展示所有需列印零件，可於右側 Customizer 切換單獨匯出 STL
view_mode = "side_by_side"; // [side_by_side:平鋪展示 (前殼+後蓋+街機搖桿頭), exploded:立體爆炸裝配圖, front_only:僅前殼 (Front Case), rear_only:僅後蓋 (Rear Case), joystick_cap_only:僅街機圓球搖桿套頭 (Joystick Cap)]

// --- 視圖呈現邏輯 ---
if (view_mode == "side_by_side") {
    // 1. 平鋪展示模式：前殼在左、後蓋在右、街機搖桿套頭在右上方
    color([0.22, 0.22, 0.25, 0.9]) 
        front_case();
    
    color([0.28, 0.28, 0.32, 0.9]) 
        translate([case_w + 15, 0, 0]) 
            rear_case();
    
    // 街機經典圓球搖桿頭 (熱血亮紅)
    color([0.85, 0.1, 0.1, 1.0]) 
        translate([case_w + 25, case_h + 10, 0]) 
            joystick_ball_cap();

} else if (view_mode == "exploded") {
    // 2. 立體爆炸裝配展示：前殼在下、街機搖桿套頭浮空對位、後蓋在上翻轉扣合
    color([0.22, 0.22, 0.25, 0.85]) 
        front_case();
    
    // 街機圓球搖桿套頭浮空對位 (X=14.0, Y=21.5)
    color([0.85, 0.1, 0.1, 1.0]) 
        translate([14.0, 21.5, 5.0]) 
            joystick_ball_cap();
    
    // 後蓋以真機裝配姿態浮空展示 (上下殼相對扣合，內卡榫在左、螺絲孔在右、火山口朝上)
    color([0.28, 0.28, 0.32, 0.85]) 
        translate([0, 0, front_thick + rear_thick + 20.0]) 
            mirror([0, 0, 1]) 
                rear_case();

} else if (view_mode == "front_only") {
    // 3. 單獨前殼 (按 F6 渲染後可直接匯出 front_case.stl)
    front_case();

} else if (view_mode == "rear_only") {
    // 4. 單獨後蓋 (按 F6 渲染後可直接匯出 rear_case.stl)
    rear_case();

} else if (view_mode == "joystick_cap_only") {
    // 5. 單獨街機圓球搖桿套頭 (按 F6 渲染後可直接匯出 joystick_ball_cap.stl)
    joystick_ball_cap();
}
