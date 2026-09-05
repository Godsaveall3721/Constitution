#include "result.h"

static const constitution_result_t constitution_ok_value = { CONSTITUTION_STATUS_OK, "《宪法》：操作成功" };
static const constitution_result_t constitution_invalid_argument_value = { CONSTITUTION_STATUS_INVALID_ARGUMENT, "《宪法》：参数无效" };

const constitution_result_t *constitution_ok(void)
{
    return &constitution_ok_value;
}

const constitution_result_t *constitution_invalid_argument(void)
{
    return &constitution_invalid_argument_value;
}
