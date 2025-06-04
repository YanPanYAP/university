document.addEventListener("DOMContentLoaded", function () {
  const calendarDays = document.querySelectorAll(".calendar-day");

  calendarDays.forEach((day) => {
    day.addEventListener("click", function () {
      calendarDays.forEach((d) => {
        d.classList.remove("selected");
        d.querySelector("span").style.color = "#ffffff";
      });

      this.classList.add("selected");
      this.querySelector("span").style.color = "#fd8737";

      updateBookingSummary();
    });
  });

  const timeInputs = document.querySelectorAll(".time-input");

  timeInputs.forEach((input) => {
    input.addEventListener("focus", function () {
      this.setAttribute("type", "time");
      this.style.color = "#ffffff";
    });

    input.addEventListener("blur", function () {
      if (!this.value) {
        this.setAttribute("type", "text");
        this.style.color = "#676664";
      } else {
        this.style.color = "#ffffff";
      }
      updateBookingSummary();
    });
  });

  const formInputs = document.querySelectorAll(".form-input");

  formInputs.forEach((input) => {
    input.addEventListener("focus", function () {
      this.style.color = "#ffffff";
    });

    input.addEventListener("blur", function () {
      if (!this.value) {
        this.style.color = "#676664";
      } else {
        this.style.color = "#ffffff";
      }
    });
  });

  const rentBtn = document.querySelector(".rent-btn");

  rentBtn.addEventListener("click", function () {
    const nameInput = document.querySelector('.form-input[placeholder="Name"]');
    const emailInput = document.querySelector(
      '.form-input[placeholder="Email"]'
    );
    const phoneInput = document.querySelector(
      '.form-input[placeholder="Phone"]'
    );

    let isValid = true;

    if (!nameInput.value) {
      nameInput.style.borderColor = "#fd8737";
      isValid = false;
    } else {
      nameInput.style.borderColor = "";
    }

    if (!emailInput.value || !emailInput.value.includes("@")) {
      emailInput.style.borderColor = "#fd8737";
      isValid = false;
    } else {
      emailInput.style.borderColor = "";
    }

    if (!phoneInput.value) {
      phoneInput.style.borderColor = "#fd8737";
      isValid = false;
    } else {
      phoneInput.style.borderColor = "";
    }

    if (isValid) {
      alert(
        "Booking successful! We will contact you shortly to confirm your reservation."
      );

      nameInput.value = "";
      emailInput.value = "";
      phoneInput.value = "";

      timeInputs.forEach((input) => {
        input.value = "";
        input.setAttribute("type", "text");
        input.style.color = "#676664";
      });
    } else {
      alert("Please fill in all required fields correctly.");
    }
  });

  const photographerRentBtns = document.querySelectorAll(
    ".rent-photographer-btn"
  );

  photographerRentBtns.forEach((btn) => {
    btn.addEventListener("click", function () {
      const photographerName =
        this.closest(".photographer-card").querySelector(
          ".photographer-name"
        ).textContent;
      alert(
        `You've selected photographer ${photographerName}. Please fill in the booking form to complete your reservation.`
      );
    });
  });

  const prevMonthBtn = document.querySelector(".month-nav-btn:first-child");
  const nextMonthBtn = document.querySelector(".month-nav-btn:last-child");
  const monthName = document.querySelector(".month-name");

  const months = [
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
  ];
  let currentMonthIndex = months.indexOf("May");

  prevMonthBtn.addEventListener("click", function () {
    currentMonthIndex = (currentMonthIndex - 1 + 12) % 12;
    monthName.textContent = months[currentMonthIndex];

    simulateCalendarUpdate();
  });

  nextMonthBtn.addEventListener("click", function () {
    currentMonthIndex = (currentMonthIndex + 1) % 12;
    monthName.textContent = months[currentMonthIndex];

    simulateCalendarUpdate();
  });

  const newsletterBtn = document.querySelector(".newsletter-btn");
  const newsletterInput = document.querySelector(".newsletter-input");

  newsletterBtn.addEventListener("click", function () {
    if (newsletterInput.value && newsletterInput.value.includes("@")) {
      alert(
        `Thank you for subscribing to our newsletter with email: ${newsletterInput.value}`
      );
      newsletterInput.value = "";
    } else {
      alert("Please enter a valid email address.");
      newsletterInput.focus();
    }
  });

  const marqueeContent = document.querySelector(".marquee-content");
  const marqueeContentClone = marqueeContent.cloneNode(true);
  document.querySelector(".marquee").appendChild(marqueeContentClone);

  function updateBookingSummary() {
    const selectedDay = document.querySelector(".calendar-day.selected");
    const fromTime = document.querySelector(
      '.time-input[placeholder="From"]'
    ).value;
    const toTime = document.querySelector(
      '.time-input[placeholder="To"]'
    ).value;

    if (selectedDay && fromTime && toTime) {
      console.log(
        `Booking summary: Yellow Mood studio on ${monthName.textContent} ${selectedDay.dataset.day}, from ${fromTime} to ${toTime}`
      );
    }
  }

  function simulateCalendarUpdate() {
    calendarDays.forEach((day) => {
      const isAvailable = Math.random() > 0.3;

      if (!isAvailable) {
        day.style.opacity = "0.5";
        day.style.cursor = "not-allowed";
      } else {
        day.style.opacity = "1";
        day.style.cursor = "pointer";
      }

      day.classList.remove("selected");
      day.querySelector("span").style.color = "#ffffff";
    });
  }
});
