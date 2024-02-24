<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <!-- Шаблон соответствующий корневому элементу -->
  <xsl:template match="/">
    <!-- Формирование структуры HTML документа -->
    <html>
      <head>
        <title>results</title>
      </head>
      <body>
        <h2>results</h2>
        <!-- Создание таблицы с границей 2px -->
        <table border="2px">
          <tr>
            <th>Student</th>
            <th>Mark</th>
          </tr>
          <!-- Итерация по каждому элементу <student> внутри <catalog> -->
          <xsl:for-each select="catalog/student">
            <!-- Сортировка по атрибуту 'name' в алфавитном порядке -->
            <xsl:sort select="name"/>
            <tr>
              <td>
                <!-- Вывод значения элемента <name> -->
                <xsl:value-of select="name"/>
              </td>
              <!-- Условная конструкция для определения цвета фона в зависимости от значения элемента <mark> -->
              <xsl:choose>
                <!-- Если значение <mark> больше '8' -->
                <xsl:when test="mark&gt;'8'">
                  <td bgcolor="green">
                    <!-- Вывод значения элемента <mark> -->
                    <xsl:value-of select="mark"/>
                  </td>
                </xsl:when>
                <!-- В противном случае -->
                <xsl:otherwise>
                  <!-- Вложенная условная конструкция для определения цвета фона в зависимости от значения <mark> -->
                  <xsl:choose>
                    <!-- Если значение <mark> меньше '4' -->
                    <xsl:when test="mark&lt;'4'">
                      <td bgcolor="red">
                        <!-- Вывод значения элемента <mark> -->
                        <xsl:value-of select="mark"/>
                      </td>
                    </xsl:when>
                    <!-- В противном случае -->
                    <xsl:otherwise>
                      <!-- Вывод значения элемента <mark> -->
                      <td>
                        <xsl:value-of select="mark"/>
                      </td>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:otherwise>
              </xsl:choose>
            </tr>
          </xsl:for-each>
        </table>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
