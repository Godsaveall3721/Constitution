# 公社公民姓名池

本目录只为行政区划中的公社建立虚构公民样本，不为大区、省、省区或广域市另建公民名册。每个公社配置10名样本公民，姓名按所在地区的本土语言习惯生成；这些记录不是现实人物或现任官员。

- `region/<大区 ID>.csv`：该大区全部公社公民清单。

CSV 表头为 `# person_id,name,language,home_commune_id,region_id`。公民 ID 由公社稳定 ID 和序号组成；`home_commune_id` 必须对应 `administrative_districts` 中的公社 ID。
