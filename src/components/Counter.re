let toLocaleString: int => string = [%bs.raw 
  {|
    function (number) {
      return number.toLocaleString()
    }
  |}
]