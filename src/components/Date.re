let parseDate: string => string = [%bs.raw
  {|
  function (timestamp) {
    var months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
    var dateObj = new Date(timestamp)
    var currentYear = new Date().getFullYear()
    var year = dateObj.getFullYear()
    var month = dateObj.getMonth()
    var date = dateObj.getDate()

    if (year === currentYear) {
      return [months[month], date].join(' ').trim()
    }
    return [months[month], date, year].join(' ').trim()
  }
|}
];