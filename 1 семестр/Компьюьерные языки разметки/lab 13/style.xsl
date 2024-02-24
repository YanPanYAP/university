<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <!-- Определение правил для сортировки -->
  <xsl:template match="telo">
    <!-- Копирование элемента <telo> -->
    <xsl:copy>
      <!-- Применение шаблона к элементу <heading> с сортировкой по возрастанию -->
      <xsl:apply-templates select="heading">
        <xsl:sort select="." order="ascending" />
      </xsl:apply-templates>
      <!-- Применение шаблона к элементу <text> -->
      <xsl:apply-templates select="text"/>
    </xsl:copy>
  </xsl:template>

  <!-- Копирование элементов, не участвующих в сортировке -->
  <xsl:template match="heading | text">
    <!-- Копирование элемента с его содержимым -->
    <xsl:copy>
      <!-- Применение шаблона к содержимому элемента -->
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
