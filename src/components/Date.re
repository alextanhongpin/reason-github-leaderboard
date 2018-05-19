let parseDate: string => string = [%bs.raw
  {|
  function (timestamp) {
    const months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
    const dateObj = new Date(timestamp)
    const currentYear = new Date().getFullYear()
    const year = dateObj.getFullYear()
    const month = dateObj.getMonth()
    const date = dateObj.getDate()

    if (year === currentYear) {
      return `${months[month]} ${date}`
    }
    return `${months[month]} ${date} ${year}`
  }
|}
];