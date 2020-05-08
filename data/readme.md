# Data編號說明備忘錄

---

## 開迴路與閉迴路實驗
- 開迴路檔案標記(marker)
    ol_n{N_STEP}_{編號}
- 激磁角I回饋標記(marker)
    cl_n{N_STEP}_{編號}
- 激磁角I回饋+電流PI回饋
    cl_in{N_STEP}_{編號}

---

## 馬達等效電感、電阻實驗
- 相位A檔案標記(marker)
    step_a_{編號}
- 相位B檔案標記(marker)
    step_b_{編號}

編號0~5，電流都是正的  
編號6~10，電流都是負的  
輸出report檔案motor_id_phase_motor_step_{相位}_{編號}

---

## 馬達常數與其他參數建模實驗
- 馬達ID檔案標記(marker)
    motor_id_{編號}

編號0: 靜止，切換  
編號1: 瞬間切換 (溫度越來越高)  
編號2: 靜止切換 (溫度越來越低)  

---

## 編碼器資料收集
- 編碼器檔案標記(marker)
    enc_noinput_{取樣頻率_編號}

用以ID FIR filter，優化輸入參數

