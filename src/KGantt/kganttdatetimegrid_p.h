/*
 * Copyright (C) 2001-2015 Klaralvdalens Datakonsult AB.  All rights reserved.
 *
 * This file is part of the KGantt library.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef KGANTTDATETIMEGRID_P_H
#define KGANTTDATETIMEGRID_P_H

#include "kganttdatetimegrid.h"
#include "kganttabstractgrid_p.h"
#include "kganttdatetimetimeline.h"

#include <QDateTime>
#include <QBrush>

namespace KGantt {
    class Q_DECL_HIDDEN DateTimeScaleFormatter::Private
    {
    public:
        Private( DateTimeScaleFormatter::Range _range,
                 const QString& _format,
                 const QString& _templ,
                 Qt::Alignment _alignment )
            : range( _range ),
              format( _format ),
              templ( _templ ),
              alignment( _alignment )
        {
        }

        const DateTimeScaleFormatter::Range range;
        const QString format;
        const QString templ;
        const Qt::Alignment alignment;
    };

    class Q_DECL_HIDDEN DateTimeGrid::Private : public AbstractGrid::Private {
    public:
        Private()
            : startDateTime( QDateTime::currentDateTime().addDays( -3 ) ),
              dayWidth( 100. ),
              scale( ScaleAuto ),
              weekStart( Qt::Monday ),
              freeDays( QSet<Qt::DayOfWeek>() << Qt::Saturday << Qt::Sunday ),
              rowSeparators( false ),
              noInformationBrush( Qt::red, Qt::DiagCrossPattern ),
              freeDaysBrush(QBrush()),
              upper( new DateTimeScaleFormatter( DateTimeScaleFormatter::Week, QString::fromLatin1( "w" ) ) ),
              lower( new DateTimeScaleFormatter( DateTimeScaleFormatter::Day, QString::fromLatin1( "ddd" ) ) ),
              year_upper( DateTimeScaleFormatter::Year, QString::fromLatin1("yyyy" ) ),
              year_lower( DateTimeScaleFormatter::Month, QString::fromLatin1("MMM" ) ),
              month_upper( DateTimeScaleFormatter::Month, QString::fromLatin1("MMMM" ) ),
              month_lower( DateTimeScaleFormatter::Week, QString::fromLatin1("w" ) ),
              week_upper( DateTimeScaleFormatter::Week, QString::fromLatin1("w" ) ),
              week_lower( DateTimeScaleFormatter::Day, QString::fromLatin1("ddd" ) ),
              day_upper( DateTimeScaleFormatter::Day, QString::fromLatin1("dddd" ) ),
              day_lower( DateTimeScaleFormatter::Hour, QString::fromLatin1("hh" ) ),
              hour_upper( DateTimeScaleFormatter::Hour, QString::fromLatin1("hh" ) ),
              hour_lower( DateTimeScaleFormatter::Minute, QString::fromLatin1("m" ) ),
              minute_upper( DateTimeScaleFormatter::Minute, QString::fromLatin1("m" ) ),
              minute_lower( DateTimeScaleFormatter::Second, QString::fromLatin1("s" ) ),
              timeLine(new DateTimeTimeLine)
        {
        }
        ~Private()
        {
            delete lower;
            delete upper;
            delete timeLine;
        }

        qreal dateTimeToChartX( const QDateTime& dt ) const;
        QDateTime chartXtoDateTime( qreal x ) const;

        int tabHeight( const QString& txt, QWidget* widget = nullptr ) const;
        void getAutomaticFormatters( DateTimeScaleFormatter** lower, DateTimeScaleFormatter** upper);
        void getFormatters( DateTimeScaleFormatter** lower, DateTimeScaleFormatter** upper);

        class DateTextFormatter {
        public:
            virtual ~DateTextFormatter() {}
            virtual QString format( const QDateTime& dt ) = 0;
            virtual QRect   textRect( qreal x, qreal offset, qreal dayWidth, const QRectF& headerRect, const QDateTime& dt ) = 0;
        };

        /*!
          * We need this because we have a header type for a year, but no such scale.
          */
        enum HeaderType {
            HeaderHour,
            HeaderDay,
            HeaderWeek,
            HeaderMonth,
            HeaderYear
        };

        HeaderType headerTypeForScale( DateTimeGrid::Scale scale );

        void paintHeader( QPainter* painter,
                          const QRectF& headerRect, const QRectF& exposedRect,
                          qreal offset, QWidget* widget,
                          HeaderType headerType,
                          DateTextFormatter *formatter );
        void paintVerticalLines( QPainter* painter,
                                 const QRectF& sceneRect,
                                 const QRectF& exposedRect,
                                 QWidget* widget,
                                 HeaderType headerType );
        void paintVerticalUserDefinedLines( QPainter* painter,
                                            const QRectF& sceneRect,
                                            const QRectF& exposedRect,
                                            QWidget* widget );

        Qt::PenStyle gridLinePenStyle( QDateTime dt, HeaderType headerType ) const;
        QDateTime adjustDateTimeForHeader( QDateTime dt, HeaderType headerType ) const;

        void drawTimeLine(QPainter* painter, const QRectF& rect);

        QDateTime startDateTime;
        QDateTime endDateTime;
        qreal dayWidth;
        Scale scale;
        Qt::DayOfWeek weekStart;
        QSet<Qt::DayOfWeek> freeDays;
        bool rowSeparators;
        QBrush noInformationBrush;
        QBrush freeDaysBrush;

        DateTimeScaleFormatter* upper;
        DateTimeScaleFormatter* lower;

        DateTimeScaleFormatter year_upper;
        DateTimeScaleFormatter year_lower;
        DateTimeScaleFormatter month_upper;
        DateTimeScaleFormatter month_lower;
        DateTimeScaleFormatter week_upper;
        DateTimeScaleFormatter week_lower;
        DateTimeScaleFormatter day_upper;
        DateTimeScaleFormatter day_lower;
        DateTimeScaleFormatter hour_upper;
        DateTimeScaleFormatter hour_lower;
        DateTimeScaleFormatter minute_upper;
        DateTimeScaleFormatter minute_lower;
        DateTimeTimeLine *timeLine;
    };

    inline DateTimeGrid::DateTimeGrid( DateTimeGrid::Private* d ) : AbstractGrid( d ) {}

    inline DateTimeGrid::Private* DateTimeGrid::d_func() {
        return static_cast<Private*>( AbstractGrid::d_func() );
    }
    inline const DateTimeGrid::Private* DateTimeGrid::d_func() const {
        return static_cast<const Private*>( AbstractGrid::d_func() );
    }
}

#endif /* KGANTTDATETIMEGRID_P_H */
