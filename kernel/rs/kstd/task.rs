use c_str;
use c_void;

extern "C" {
    pub fn task_new(f : unsafe extern "C" fn (*mut c_void, *const Task) -> i64, data : *mut c_void, name : *const i8, p: TaskPriority)-> *const Task;
    pub fn task_end_current();
    pub fn task_end_wait(t: *const Task);
    pub fn task_end(t: *const Task);
}
#[allow(dead_code)]
#[derive(Debug)]
#[repr(C)]
pub enum TaskPriority{
    TaskPriorityHigh,
    TaskPriorityMedium,
    TaskPriorityLow,
    TaskPrioritySleep,
    TaskPriorityNull
}
#[allow(dead_code)]
#[derive(Debug)]
#[repr(C)]
pub enum TaskStatus{
    TaskStatusEnded,
    TaskStatusRunning,
    TaskStatusCreated,
    TaskStatusWaitIO,
    TaskStatusPaused
}
#[allow(dead_code)]
#[repr(C,packed)]
pub struct Task{
    pub next : *const Task,
    pub prev : *const Task,
    cpu_registers : *const u64,
    r#fn : extern "C" fn (*const c_void, *const Task) -> i64,
    stack_start : *const u64,
    stack_end: *const u64,
    pub name : *const i8,
    pub task_status: TaskStatus,
    pub data : *const c_void,
    time_slice : u32,
    time_slice_remaining : i32,
    pub priority : TaskPriority,
    pub waiting_on : *const Task
}
impl Task{
    pub fn new(f : unsafe extern "C" fn (*mut c_void, *const Task) -> i64, data : *mut c_void, name : &str, p : TaskPriority) ->  * const Task{
        unsafe{
            task_new(f, data, c_str(name).as_ptr(), p)
        }
    }

}
