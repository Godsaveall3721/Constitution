# 官员与议会人员名册

本目录将职位配置中的职位模板映射到虚构公民姓名池中的人员 ID；每条职位模板配置一名不同的样本任职者。该名册是数据结构样本，不表示现实任命结果。委员会委员类职位的配置行仅代表一个职位模板，不推导委员会实际席位数量。

- `nation.csv`：国家层级职位。
- `nation/region.csv`：各大区职位。
- `nation/region/<大区 ID>.csv`：该大区内省、广域市及其下级公社职位。

职位 ID 与人员 ID 分列，并保留职位名称、`basis_unit` 和人员所属公社，便于回查 `administrative_districts_and_committee_positions` 与 `politicians`。表头为 `# position_id,position_name,basis_unit,person_id,person_name,home_commune_id`。优先在职位辖区内选择人员；辖区样本不足时向同一大区回退，国家级职位可从全国公民样本中选择。
