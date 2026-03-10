document.addEventListener('DOMContentLoaded', () => {
    document.getElementById('btnTask1').addEventListener('click', async () => {
        try {
            const result = await simulateAsyncTask(2000, 'Task 1 Completed');
            displayResult(result);
        } catch (error) {
            console.error('Error executing Task 1:', error);
            displayResult('Error executing Task 1');
        }
    });
    document.getElementById('btnTask2').addEventListener('click', async () => {
        try {
            const result = await simulateAsyncTask(3000, 'Task 2 Completed');
            displayResult(result);
        } catch (error) {
            console.error('Error executing Task 2:', error);
            displayResult('Error executing Task 2');
        }
    });
    document.getElementById('btnTask3').addEventListener('click', async () => {
        try {
            const result = await simulateAsyncTask(4000, 'Task 3 Completed');
            displayResult(result);
        } catch (error) {
            console.error('Error executing Task 3:', error);
            displayResult('Error executing Task 3');
        }
    });
    function simulateAsyncTask(delay, result) {
        return new Promise((resolve, reject) => {
            setTimeout(() => {
                resolve(result);
            }, delay);
        });
    }
    function displayResult(message) {
        const taskResultsDiv = document.getElementById('taskResults');
        taskResultsDiv.innerHTML += `<p>${message}</p>`;
    }
});