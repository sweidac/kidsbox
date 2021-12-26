console.log('hi');

const elements = {};

elements.rfidId = document.querySelector('.rfid-id');
elements.rfidRefresh = document.querySelector('.rfid-refresh');

elements.rfidRefresh.addEventListener('click', async () => {
	const response = await fetch('/rfid'),
		rfidId = await response.text();

	elements.rfidId.innerText = rfidId;
});