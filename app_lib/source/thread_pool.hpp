

namespace bq
{

	using task = std::function<void()>;

    template <class T>
    class shared_state
    {
        std::optional<T> m_result;
        std::binary_semaphore m_ready_signal {0};
    public:
        template <typename R>
        void set(R&& result)
        {
            if (!m_result)
            {
                m_result = std::forward<R>(result);
                m_ready_signal.release();
            }
        }
        T get()
        {
            m_ready_signal.acquire();
            return std::move(*m_result);
        }
    };
    template <class T>
    class promise {

    };

    class thread_pool
    {

        class worker
        {
            
            thread_pool* m_pool;
            
            std::jthread m_thread;

            void run_kernel(std::stop_token st)
            {
                while (auto task = m_pool->get_task(st))
                {
                    task();
                }
            }
            
        public:
            
            worker(thread_pool* pool) : m_pool{ pool }, m_thread(std::bind_front(&worker::run_kernel, this)) {}
            
            void request_stop()
            {
                m_thread.request_stop();
            }

        };

        // data
        std::mutex m_task_queue_mtx;
        std::condition_variable_any m_task_queue_cv;
        std::condition_variable m_done_cv;
        std::deque<task> m_tasks;
        std::vector<worker> m_workers;

        task get_task(std::stop_token& st)
        {
            task task;
            std::unique_lock lk{ m_task_queue_mtx };
            m_task_queue_cv.wait(lk, st, [this] {return !m_tasks.empty(); });
            if (!st.stop_requested()) {
                task = std::move(m_tasks.front());
                m_tasks.pop_front();
                if (m_tasks.empty()) {
                    m_done_cv.notify_all();
                }
            }
            return task;
        }
      
    public:

        thread_pool(size_t numWorkers)
        {
            m_workers.reserve(numWorkers);
            for (size_t i = 0; i < numWorkers; i++) {
                m_workers.emplace_back(this);
            }
        }

        void run(task task)
        {
            {
                std::lock_guard lk{ m_task_queue_mtx };
                m_tasks.push_back(std::move(task));
            }
            m_task_queue_cv.notify_one();
        }

        void wait_for_all()
        {
            std::unique_lock lk{ m_task_queue_mtx };
            m_done_cv.wait(lk, [this] {return m_tasks.empty(); });
        }

        ~thread_pool()
        {
            for (auto& w : m_workers) {
                w.request_stop();
            }
        }
    };


}